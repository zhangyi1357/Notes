#include <iostream>

#include "hello.h"

int main() {
    HelloModule::LoadLibrary();
    HelloModule::Foo();
    int bar = HelloModule::GetBar();
    std::cout << "bar == " << bar << std::endl;

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

    HelloModule::ReloadLibrary();
    HelloModule::Foo();
    std::cout << "bar == " << HelloModule::GetBar() << std::endl;
    return 0;
}
