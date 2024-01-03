#pragma once

#include <dlfcn.h>

#include <cstdio>

namespace Replex {

inline void* Load(const char* filepath) {
    return dlopen(filepath, RTLD_LAZY);
}

inline void* LoadSymbol(void* library, const char* symbol) {
    return dlsym(library, symbol);
}

inline void Reload(void*& library, const char* filepath) {
    if (library) {
        dlclose(library);
    }
    library = Load(filepath);
}

inline void PrintError() {
    fprintf(stderr, "%s\n", dlerror());
}

}  // namespace Replex