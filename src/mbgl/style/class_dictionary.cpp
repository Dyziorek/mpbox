#include <mbgl/style/class_dictionary.hpp>

#include <uv.h>

#include <thread>

namespace mbgl {

ClassDictionary::ClassDictionary() {}

ClassDictionary &ClassDictionary::Get() {
    // Note: We should eventually switch to uv_key_* functions, but libuv 0.10 doesn't have these
    // yet. Instead, we're using the pthread functions directly for now.
	static uv_key_t uv_keyData;
	static uv_once_t uv_guard = UV_ONCE_INIT;
	


	uv_once(&uv_guard, []()
	{
		uv_key_create(&uv_keyData);
	});

    //static pthread_once_t store_once = PTHREAD_ONCE_INIT;
    //static pthread_key_t store_key;

    //// Create the key.
    //pthread_once(&store_once, []() {
    //    pthread_key_create(&store_key, [](void *ptr) {
    //        delete reinterpret_cast<ClassDictionary *>(ptr);
    //    });
    //});

    ClassDictionary *ptr = reinterpret_cast<ClassDictionary *>(uv_key_get(&uv_keyData));
    if (ptr == nullptr) {
        ptr = new ClassDictionary();
        uv_key_set(&uv_keyData, ptr);
    }

    return *ptr;
}

ClassID ClassDictionary::lookup(const std::string &class_name) {
    auto it = store.find(class_name);
    if (it == store.end()) {
        // Insert the class name into the store.
        ClassID id = ClassID(uint32_t(ClassID::Named) + offset++);
        store.emplace(class_name, id);
        return id;
    } else {
        return it->second;
    }
}

ClassID ClassDictionary::normalize(ClassID id) {
    if (id >= ClassID::Named) {
        return ClassID::Named;
    } else {
        return id;
    }
}

}
