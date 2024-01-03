#include <cstdio>
#include <string>

#include "replex.h"

const char* g_libPath = "libhello.so";

int main() {
    void* handle;
    void (*foo)();
    int bar;

    handle = Replex::Load(g_libPath);
    if (!handle) {
        Replex::PrintError();
        return -1;
    }
    foo = reinterpret_cast<void (*)()>(Replex::LoadSymbol(handle, "foo"));
    foo();
    bar = *reinterpret_cast<int*>(Replex::LoadSymbol(handle, "bar"));
    printf("bar == %d\n", bar);

    // Modify the source code and recompile the library.
    std::string filename = "hello.cpp";
    std::string command = std::string("sed -i ") +
                          (bar == 200 ? "'s/200/300/'" : "'s/300/200/'") + " " +
                          filename;
    system(command.c_str());
    command = std::string("sed -i ") +
              (bar == 200 ? "'s/Hi/Hello/'" : "'s/Hello/Hi/'") + " " + filename;
    system(command.c_str());
    system("g++ -shared -fPIC -o libhello.so hello.cpp");

    Replex::Reload(handle, g_libPath);
    if (!handle) {
        Replex::PrintError();
        return -1;
    }
    foo = reinterpret_cast<void (*)()>(Replex::LoadSymbol(handle, "foo"));
    foo();
    bar = *reinterpret_cast<int*>(Replex::LoadSymbol(handle, "bar"));
    printf("bar == %d\n", bar);

    return 0;
}