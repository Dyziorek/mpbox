#include <mbgl/storage/asset_context.hpp>
#include <mbgl/storage/resource.hpp>
#include <mbgl/storage/response.hpp>
#include <mbgl/util/std.hpp>
#include <mbgl/util/util.hpp>
#include <mbgl/util/url.hpp>
#include <mbgl/util/uv.hpp>

#include <uv.h>

#include <fcntl.h>
#include <cassert>
#include <limits>

namespace mbgl {

class AssetRequest : public RequestBase {
    MBGL_STORE_THREAD(tid)

public:
    AssetRequest(const Resource&, Callback, uv_loop_t*, const std::string& assetRoot);
    ~AssetRequest();

    void cancel() override;

    static void fileOpened(uv_fs_t *req);
    static void fileStated(uv_fs_t *req);
    static void fileRead(uv_fs_t *req);
    static void fileClosed(uv_fs_t *req);
    static void notifyError(uv_fs_t *req);
    static void cleanup(uv_fs_t *req);

    std::string assetRoot;
    bool canceled = false;
    uv_fs_t req;
    uv_file fd = -1;
    uv_buf_t buffer;
    std::unique_ptr<Response> response;
};

class AssetFSContext : public AssetContext {
    RequestBase* createRequest(const Resource& resource,
                               RequestBase::Callback callback,
                               uv_loop_t* loop,
                               const std::string& assetRoot) override {
        return new AssetRequest(resource, callback, loop, assetRoot);
    }
};

AssetRequest::~AssetRequest() {
    MBGL_VERIFY_THREAD(tid);
}

AssetRequest::AssetRequest(const Resource& resource_, Callback callback_, uv_loop_t* loop, const std::string& assetRoot_)
    : RequestBase(resource_, callback_),
      assetRoot(assetRoot_) {
    req.data = this;

    const auto &url = resource.url;
    std::string path;
    if (url.size() <= 8 || url[8] == '/') {
        // This is an empty or absolute path.
        path = mbgl::util::percentDecode(url.substr(8));
    } else {
        // This is a relative path. Prefix with the application root.
        path = assetRoot + mbgl::util::percentDecode(url.substr(8));
    }

    uv_fs_open(loop, &req, path.c_str(), _O_RDONLY, _S_IREAD, fileOpened);
}

void AssetRequest::fileOpened(uv_fs_t *req) {
    assert(req->data);
    auto self = reinterpret_cast<AssetRequest *>(req->data);
    MBGL_VERIFY_THREAD(self->tid);

    if (req->result < 0) {
        // Opening failed or was canceled. There isn't much left we can do.
        notifyError(req);
        cleanup(req);
    } else {
        const uv_file fd = uv_file(req->result);

        // We're going to reuse this handle, so we need to cleanup first.
        uv_fs_req_cleanup(req);

        if (self->canceled) {
            // The request was canceled.
            uv_fs_close(req->loop, req, fd, fileClosed);
        } else {
            self->fd = fd;
            uv_fs_fstat(req->loop, req, fd, fileStated);
        }
    }
}

void AssetRequest::fileStated(uv_fs_t *req) {
    assert(req->data);
    auto self = reinterpret_cast<AssetRequest *>(req->data);
    MBGL_VERIFY_THREAD(self->tid);

    if (req->result != 0 || self->canceled) {
        // Stating failed or was canceled. We already have an open file handle
        // though, which we'll have to close.
        notifyError(req);

        uv_fs_req_cleanup(req);
        uv_fs_close(req->loop, req, self->fd, fileClosed);
    } else {
#if UV_VERSION_MAJOR == 0 && UV_VERSION_MINOR <= 10
        auto stat = static_cast<const uv_statbuf_t *>(req->ptr);
#else
        auto stat = static_cast<const uv_stat_t *>(req->ptr);
#endif
        if (stat->st_size > std::numeric_limits<int>::max()) {
            // File is too large for us to open this way because uv_buf's only support unsigned
            // ints as maximum size.
            auto response = util::make_unique<Response>();
            response->status = Response::Error;
#if UV_VERSION_MAJOR == 0 && UV_VERSION_MINOR <= 10
            response->message = uv_strerror(uv_err_t {UV_EFBIG, 0});
#else
            response->message = uv_strerror(UV_EFBIG);
#endif
            self->notify(std::move(response), FileCache::Hint::No);

            uv_fs_req_cleanup(req);
            uv_fs_close(req->loop, req, self->fd, fileClosed);
        } else {
            self->response = util::make_unique<Response>();
#ifdef __APPLE__
            self->response->modified = stat->st_mtimespec.tv_sec;
#else
            self->response->modified = stat->st_mtim.tv_sec;
#endif
            self->response->etag = std::to_string(stat->st_ino);
            const auto size = (unsigned int)(stat->st_size);
            self->response->data = self->response->data.append(size, 0);
            self->buffer = uv_buf_init(const_cast<char *>(self->response->data.data()), size);
            uv_fs_req_cleanup(req);
#if UV_VERSION_MAJOR == 0 && UV_VERSION_MINOR <= 10
            uv_fs_read(req->loop, req, self->fd, self->buffer.base, self->buffer.len, -1, fileRead);
#else
            uv_fs_read(req->loop, req, self->fd, &self->buffer, 1, 0, fileRead);
#endif
        }
    }
}

void AssetRequest::fileRead(uv_fs_t *req) {
    assert(req->data);
    auto self = reinterpret_cast<AssetRequest *>(req->data);
    MBGL_VERIFY_THREAD(self->tid);

    if (req->result < 0 || self->canceled) {
        // Stating failed or was canceled. We already have an open file handle
        // though, which we'll have to close.
        notifyError(req);
    } else {
        // File was successfully read.
        self->response->status = Response::Successful;
		self->response->type = "Asset_FS";
        self->notify(std::move(self->response), FileCache::Hint::No);
    }

    uv_fs_req_cleanup(req);
    uv_fs_close(req->loop, req, self->fd, fileClosed);
}

void AssetRequest::fileClosed(uv_fs_t *req) {
    assert(req->data);
    auto self = reinterpret_cast<AssetRequest *>(req->data);
    MBGL_VERIFY_THREAD(self->tid);
    (void(self)); // Silence unused variable error in Release mode

    if (req->result < 0) {
        // Closing the file failed. But there isn't anything we can do.
    }

    cleanup(req);
}

void AssetRequest::notifyError(uv_fs_t *req) {
    assert(req->data);
    auto self = reinterpret_cast<AssetRequest *>(req->data);
    MBGL_VERIFY_THREAD(self->tid);

    if (req->result < 0 && !self->canceled && req->result != UV_ECANCELED) {
        auto response = util::make_unique<Response>();
        response->status = Response::Error;
        response->message = uv::getFileRequestError(req);
        self->notify(std::move(response), FileCache::Hint::No);
    }
}

void AssetRequest::cleanup(uv_fs_t *req) {
    assert(req->data);
    auto self = reinterpret_cast<AssetRequest *>(req->data);
    MBGL_VERIFY_THREAD(self->tid);
    uv_fs_req_cleanup(req);
    delete self;
}

void AssetRequest::cancel() {
    canceled = true;
    // uv_cancel fails frequently when the request has already been started.
    // In that case, we have to let it complete and check the canceled bool
    // instead. The cancelation callback will delete the AssetRequest object.
    uv_cancel((uv_req_t *)&req);
}

std::unique_ptr<AssetContext> AssetContext::createContext(uv_loop_t*) {
    return util::make_unique<AssetFSContext>();
}

}
