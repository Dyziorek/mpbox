#include <mbgl/platform/platform.hpp>

#include <uv.h>
//#include <libgen.h>

namespace mbgl {
namespace platform {

// Returns the path to the root folder of the application.
const std::string &applicationRoot() {
    static const std::string root = []() -> std::string {
        size_t max = 0;
        std::string dir;
		std::string drive;
		std::string file;
		std::string ext;
        do {
            // Gradually increase the length of the string in case the path was truncated.
            max += 256;
			dir.resize(max);
			drive.resize(max);
			file.resize(max);
			ext.resize(max);
            uv_exepath(const_cast<char *>(dir.data()), &max);
        } while (max == dir.size());

		size_t dirc = max -1, extc = max - 1, drvc = max - 1, filec = max -1;
        _splitpath_s(const_cast<char *>(dir.c_str()), const_cast<char*>(drive.data()), drvc, const_cast<char*>(dir.data()), dirc, const_cast<char*>(file.data()), filec, const_cast<char*>(ext.data()), extc);
        return dir;
    }();
    return root;
}

}
}
