# C++ 动态库热加载

本文参考自 [project-based-learning](https://github.com/practical-tutorials/project-based-learning) 中的 [Build a Live Code-reloader Library for C++](https://howistart.org/posts/cpp/1/index.html)，主要内容都来自于其中，但是对代码进行了一点修改，并且改用 CMake 进行构建。

文章整体比较基础，适合初学者，通过本文可以学习到以下知识点

1. 关于 C++ 程序如何编译运行，如何运行时加载动态库（使用 `dl*` API）。
2. 如何设计简洁易用的库 API 供用户使用。
3. 如何使用 CMake 组织并构建一个包含可执行程序、动态库和头文件库的项目。
4. 如何使用 GoogleTest 进行测试。

## 动态库热加载原理

动态库热加载指的是在程序运行时，动态地加载动态库，从而达到不停止程序的情况下，更新程序的功能。

C++ 程序在运行时有两种方式加载动态连接库：隐式链接和显式链接 [^1]。

1. 隐式链接就是在编译的时候使用 `-l` 参数链接的动态库，进程在开始执行时就将动态库文件映射到内存空间中。
2. 显式链接使用 `libdl.so` 库的 API 接口在运行中加载和卸载动态库，主要的 API 有 `dlopen、dlclose、dlsym、dlerror`。

隐式链接的方式要进行热加载需要不少 Hack，难度较大，本文主要讲解第二种方式。

[^1]: [Linux 下 C++so 热更新](https://zhuanlan.zhihu.com/p/162366167)

## 简单版本

首先我们快速实现一个能够完成最小功能可运行的版本，熟悉相关 API 的使用。我们简单编写三个文件，分别为`main.cpp`， `replex.h`，`hello.cpp`，另外还编写一个快速编译运行代码的脚本 `run.sh`，目录结构如下

```bash
.
├── hello.cpp
├── main.cpp
├── replex.h
└── run.sh
```

代码的完整版本见 [projects/replex-1](https://github.com/zhangyi1357/Notes/tree/main/projects/replex-1)。

`replex.h` 中对 `dl*` API 进行了简单的封装，使用一个 namespace 将 API 进行了包装，代码如下

```cpp
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
```

`hello.cpp` 是我们需要热加载的动态库，代码如下

```cpp
#include <cstdio>

extern "C" {
void foo() {
    printf("Hi\n");
}

int bar = 200;
}
```

其中使用 `extern "C"` 将 `foo` 和 `bar` 声明为 C 语言的函数和变量，这样在编译时就不会对函数名进行修饰，否则在 `main.cpp` 中使用 `dlsym` 时会找不到 `foo` 对应的符号。

不加 `extern "C"`时，使用 `nm` 命令查看 `hello.so` 中的符号如下

```bash
$ nm libhello.so  | grep foo
0000000000001119 T _Z3foov
```

加上后

```bash
$ nm libhello.so  | grep foo
0000000000001119 T foo
```

`main.cpp` 是主程序，代码如下

```cpp
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
```

整体代码逻辑比较好懂，首先加载动态库，然后获取动态库中的函数和变量，调用函数和打印变量，然后修改 `hello.cpp` 中的代码，重新编译动态库，再次加载动态库，调用函数和打印变量。

`reinterpret_cast` 是 C++ 中的强制类型转换，将 `void*` 指针转换为函数指针和变量指针。

`run.sh` 的内容如下

```bash
#!/bin/bash
set -e # stop the script on errors
g++ -fPIC -shared -o libhello.so hello.cpp
g++ -o main.out main.cpp -ldl
./main.out
```

脚本中 `-shared -fPIC` 参数用于生成位置无关的动态库，`-ldl` 参数用于链接 `libdl.so` 库（`dl*` API），`-o` 参数用于指定输出文件名。

运行脚本后，输出如下

```bash
Hi
bar == 200
Hello
bar == 300
```

当前程序能够完成基本功能，但是对于使用者来说我们的库不够好用，使用者（`main.cpp`）需要自己定义相应的函数指针和类型，还需要自己进行类型转换，动态库的导出符号也需要自己定义，对于使用者来说也相当麻烦。

## 改进版本

我们考虑提供更简单的接口供用户使用，我们将在 `replex.h` 中创建一个 `ReplexModule` 类，这个类将用于给动态库的继承使用，然后由动态库的作者提供更加简明的接口供用户使用。

这一版本代码的完整实现见 [GitHub](https://github.com/zhangyi1357/Notes/tree/main/projects/replex-2)。

最终的使用效果见如下 `main.cpp` 文件

```cpp
#include <iostream>

#include "hello.h"

int main() {
    HelloModule::LoadLibrary();
    HelloModule::Foo();
    int bar = HelloModule::GetBar();
    std::cout << "bar == " << bar << std::endl;

    // Modify the source code and recompile the library.
    // ...

    HelloModule::ReloadLibrary();
    HelloModule::Foo();
    std::cout << "bar == " << HelloModule::GetBar() << std::endl;
    return 0;
}
```

我们忽略中间的修改源码和重新编译的过程，这里只关注 `HelloModule` 的使用，相比于前一版本，这里的使用更加简单，不需要自己定义函数指针和变量，也不需要自己进行类型转换，只需要调用 `HelloModule` 中的接口即可。同时注意到我们包含的头文件也变成了 `hello.h`，这个头文件是动态库作者提供的，我们在 `main.cpp` 中只需要包含这个头文件即可。

针对于上述需求，`ReplexModule` 需要公开两个公共接口，一个用于发布可热加载库，另一个用于加载和重新加载这些可热加载库。

`ReplexModule` 的公开接口仅有两个，分别为 `LoadLibrary` 和 `ReloadLibrary`，代码如下

```cpp
#pragma once

#include <dlfcn.h>

#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

template <typename E, size_t NumSymbols>
class ReplexModule {
   public:
    static void LoadLibrary() { GetInstance().Load(); }
    static void ReloadLibrary() { GetInstance().Reload(); }

   protected:
    static E& GetInstance() {
        static E instance;
        return instance;
    }

    // ...
    // ... continued later
}
```

这两个函数都依赖于 `GetInstance` 函数，这个函数是一个模板函数，用于返回 `ReplexModule` 的子类的单例，这样可以保证每个子类只有一个实例。另外，`ReplexModule` 是一个模板类，模板参数 `E` 是一个枚举类型，用于指定动态库中的符号，`NumSymbols` 是一个常量，用于指定动态库中的符号个数。

接下来关注 `ReplexModule` 向动态库作者也就是集成该类的子类提供的接口，代码如下：

```cpp
    // ... continued above

    // Should return the path to the library on disk
    virtual const char* GetPath() const = 0;

    // Should return a reference to an array of C-strings of size NumSymbols
    // Used when loading or reloading the library to lookup the address of
    // all exported symbols
    virtual std::array<const char*, NumSymbols>& GetSymbolNames() const = 0;

    template <typename Ret, typename... Args>
    Ret Execute(const char* name, Args... args) {
        // Lookup the function address
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end()) {
            // Cast the address to the appropriate function type and call it,
            // forwarding all arguments
            return reinterpret_cast<Ret (*)(Args...)>(symbol->second)(args...);
        }
        throw std::runtime_error(std::string("Function not found: ") + name);
    }

    template <typename T>
    T* GetVar(const char* name) {
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end()) {
            return static_cast<T*>(symbol->second);
        }
        // We didn't find the variable. Return an empty pointer
        return nullptr;
    }

   private:
    void Load() {
        m_libHandle = dlopen(GetPath(), RTLD_NOW);
        LoadSymbols();
    }

    void Reload() {
        auto ret = dlclose(m_libHandle);
        m_symbols.clear();
        Load();
    }

    void LoadSymbols() {
        for (const char* symbol : GetSymbolNames()) {
            auto* sym = dlsym(m_libHandle, symbol);
            m_symbols[symbol] = sym;
        }
    }

    void* m_libHandle;
    std::unordered_map<std::string, void*> m_symbols;
};
```

首先关注最底部的数据成员，`m_libHandle` 是动态库的句柄，`m_symbols` 是一个哈希表，用于存储动态库中的符号和符号对应的地址。 `Load` 函数用于加载动态库，`Reload` 函数用于重新加载动态库，`LoadSymbols` 函数用于加载动态库中的符号，这几个函数的逻辑相当清晰无需赘述。

值得讲解的是 `Execute` 和 `GetVar` 函数，`Execute` 函数用于调用动态库中的函数，`GetVar` 函数用于获取动态库中的变量，让我们先看看 `Execute` 函数的实现，代码如下

```cpp
    template <typename Ret, typename... Args>
    Ret Execute(const char* name, Args... args) {
        // Lookup the function address
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end()) {
            // Cast the address to the appropriate function type and call it,
            // forwarding all arguments
            return reinterpret_cast<Ret (*)(Args...)>(symbol->second)(args...);
        }
        throw std::runtime_error(std::string("Function not found: ") + name);
    }
```

这是一个模板函数，模板参数 `Ret` 是返回值类型，`Args...` 是参数类型，这里的 `Args...` 表示可以接受任意多个参数，`Args... args` 表示将参数包 `args` 展开，然后将展开后的参数作为参数传递给 `Execute` 函数。

该函数首先在 `m_symbols` 中查找 `name` 对应的符号，如果找到了，就将符号地址转换为类型为 `Ret (*)(Args...)` 的函数指针，然后调用该函数，传递参数 `args...`，如果没有找到，就抛出异常。

`GetVar` 函数的实现如下

```cpp
    template <typename T>
    T* GetVar(const char* name) {
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end()) {
            return static_cast<T*>(symbol->second);
        }
        // We didn't find the variable. Return an empty pointer
        return nullptr;
    }
```

该函数的实现和 `Execute` 函数类似，只是将函数指针转换为变量指针，然后返回。

`hello.cpp` 的内容保持不变：

```cpp
#include <cstdio>

extern "C" {
void foo() {
    printf("Hi\n");
}

int bar = 200;
}
```

`hello.h` 中定义类 `HelloModule` 继承自 `ReplexModule`，代码如下

```cpp
#pragma once
#include <array>

#include "replex.h"

inline std::array<const char*, 2> g_exports = {"foo", "bar"};

class HelloModule : public ReplexModule<HelloModule, g_exports.size()> {
   public:
    static void Foo() { GetInstance().Execute<void>("foo"); }

    static int GetBar() { return *GetInstance().GetVar<int>("bar"); }

   protected:
    virtual const char* GetPath() const override { return "libhello.so"; }

    virtual std::array<const char*, g_exports.size()>& GetSymbolNames()
        const override {
        return g_exports;
    }
};
```

变量 `g_exports` 用于存储动态库中需要导出的符号，其采用 `inline` 修饰，这样就可以在头文件中定义，而不会出现重复定义的错误。

`HelloModule` 中定义了两个静态函数，分别为 `Foo` 和 `GetBar`，这两个函数用于调用动态库中的函数和获取动态库中的变量。

运行脚本的内容基本不变，添加了 `-std=c++17` 的标志保证可以使用 `inline` 变量的用法。

```bash
#!/bin/bash
set -e # stop the script on errors
g++ -fPIC -shared -o libhello.so hello.cpp -std=c++17
g++ -o main.out main.cpp -ldl -std=c++17
./main.out
```

运行效果与前一版本一致，如下

```bash
Hi
bar == 200
Hello
bar == 300
```

现在我们可以认为我们所编写的 `replex.h` 库足方便使用，动态库作者只需要继承 `ReplexModule` 类，然后实现两个虚函数即可，使用者只需要包含动态库作者提供的头文件，然后调用相应的接口即可。

## CMake 版本

前面两个版本的代码都是写个脚本直接使用 `g++` 编译，这样的方式不够灵活，不利于项目的管理，正好这个项目涉及到几个不同的模块，可以尝试使用 `CMake` 进行管理，学习一下项目的组织构建。

完整代码见 [projects/replex-3](https://github.com/zhangyi1357/Notes/tree/main/projects/replex-3)，采用 [现代 CMake 模块化项目管理指南](https://github.com/zhangyi1357/Notes/blob/main/cmake-1.md) 中推荐的方式进行项目组织，但是略微进行了一点简化，目录结构如下

```bash
.
├── CMakeLists.txt
├── hello
│   ├── CMakeLists.txt
│   ├── include
│   │   └── hello.h
│   └── src
│       └── hello.cpp
├── main
│   ├── CMakeLists.txt
│   └── src
│       └── main.cpp
└── replex
    ├── CMakeLists.txt
    └── include
        └── replex.h
```

首先梳理一下整个项目的依赖关系，如下所示

```bash
main (exe)
├── hello_interface (interface)
│   └── replex (interface)
└── hello (shared lib)
```

main 模块依赖于头文件库 hello_interface，hello_interface 依赖于头文件库 replex，动态库 hello 不依赖于任何库，用于提供给 main 模块使用。

`CMakeLists.txt` 为根目录的 `CMakeLists.txt`，内容如下

```cmake
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(replex LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

add_subdirectory(replex)
add_subdirectory(main)
add_subdirectory(hello)
```

首先设置 C++ 标准，然后设置项目名称，然后判断是否设置了构建类型，如果没有设置，则设置为 Release 模式，然后添加子目录，分别为 replex、main 和 hello。

`replex/CMakeLists.txt` 的内容如下

```cmake
add_library(replex INTERFACE include/replex.h)
target_include_directories(replex INTERFACE include)
```

replex 为头文件库，使用 `add_library` 添加，类型为 INTERFACE，表示这是一个接口库，不会生成任何文件，只会导出头文件，使用 `target_include_directories` 添加头文件路径。

`hello/CMakeLists.txt` 的内容如下

```cmake
add_library(hello SHARED src/hello.cpp)

add_library(hello_interface INTERFACE include/hello.h)
target_include_directories(hello_interface INTERFACE include)
target_link_libraries(hello_interface INTERFACE replex)
```

其中定义了两个库，一个为动态库 hello，一个为头文件库 hello_interface 用于导出 动态库 hello 中的符号以供使用， hello_interface 依赖于 replex，使用 `target_link_libraries` 添加依赖。

`main/CMakeLists.txt` 的内容如下

```cmake
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE hello_interface)
```

main 为可执行文件，使用 `add_executable` 添加，使用 `target_link_libraries` 添加依赖 `hello_interface`。

最后运行脚本 `run.sh`，内容如下

```bash
#!/bin/bash
set -e # stop the script on errors
cmake -B build
cmake --build build
./build/main/main
```

运行的效果如下

```bash
Hi
bar == 200
[  0%] Built target replex
[  0%] Built target hello_interface
[ 50%] Built target main
[ 75%] Building CXX object hello/CMakeFiles/hello.dir/src/hello.cpp.o
[100%] Linking CXX shared library libhello.so
[100%] Built target hello
Hello
bar == 300
```

## 添加测试 （[GoogleTest](https://github.com/google/googletest)）

这部分的完整代码见 [projects/replex-4](https://github.com/zhangyi1357/Notes/tree/main/projects/replex-4)。

一个好的项目，测试是必不可少的，前面我们实现的 `main.cpp` 中其实已经有了一点自动化测试的影子，但是这种方式不够好，我们可以使用 GoogleTest 来进行测试。

首先演示一个最基本的 gtest 用法，首先使用 git 的 `submodule` 命令添加 googletest 到我们的项目中

```bash
git submodule add git@github.com:google/googletest.git
```

然后修改我们根目录下的 CMakeLists.txt，添加如下内容

```cmake
add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(test)
```

创建 test 目录，结构如下

```bash
test
├── CMakeLists.txt
└── src
    └── test.cpp
```

`test/CMakeLists.txt` 的内容如下

```cmake
add_executable(tests src/test.cpp)
target_link_libraries(tests PUBLIC gtest gtest_main)
```

`test/src/test.cpp` 的内容如下

```cpp
#include <gtest/gtest.h>

TEST(SillyTest, IsFourPositive) {
    EXPECT_GT(4, 0);
}

TEST(SillyTest, IsFourTimesFourSixteen) {
    int x = 4;
    EXPECT_EQ(x * x, 16);
}

int main(int argc, char** argv) {
    // This allows us to call this executable with various command line
    // arguments which get parsed in InitGoogleTest
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

OK，到现在我们已经成功添加了 GoogleTest 到我们的项目中并且可以运行测试了，现在我们要编写一些测试来测试我们的项目。

我们编写一个 replex 的测试，测试内容如下

```cpp
#include <gtest/gtest.h>
#include <hello.h>

#include <cstdlib>
#include <fstream>

const char* g_Test_v1 = R"delimiter(
extern "C" {
int foo(int x) {
    return x + 5;
}
int bar = 3;
}
)delimiter";

const char* g_Test_v2 = R"delimiter(
extern "C" {
int foo(int x) {
    return x - 5;
}
int bar = -2;
}
)delimiter";

class ReplexTest : public ::testing::Test {
   public:
    // Called automatically at the start of each test case.
    virtual void SetUp() {
        WriteFile("hello/src/hello.cpp", g_Test_v1);
        Compile(1);
        HelloModule::LoadLibrary();
    }

    // We'll invoke this function manually in the middle of each test case
    void ChangeAndReload() {
        WriteFile("hello/src/hello.cpp", g_Test_v2);
        Compile(2);
        HelloModule::ReloadLibrary();
    }

    // Called automatically at the end of each test case.
    virtual void TearDown() {
        HelloModule::UnloadLibrary();
        WriteFile("hello/src/hello.cpp", g_Test_v1);
        Compile(1);
    }

   private:
    void WriteFile(const char* path, const char* text) {
        // Open an output filetream, deleting existing contents
        std::ofstream out(path, std::ios_base::trunc | std::ios_base::out);
        out << text;
    }

    void Compile(int version) {
        if (version == m_version) {
            return;
        }

        m_version = version;
        EXPECT_EQ(std::system("cmake --build build"), 0);

        // Super unfortunate sleep due to the result of cmake not being fully
        // flushed by the time the command returns (there are more elegant ways
        // to solve this)
        sleep(1);
    }

    int m_version = 1;
};

TEST_F(ReplexTest, VariableReload) {
    EXPECT_EQ(HelloModule::GetBar(), 3);
    ChangeAndReload();
    EXPECT_EQ(HelloModule::GetBar(), -2);
}

TEST_F(ReplexTest, FunctionReload) {
    EXPECT_EQ(HelloModule::Foo(4), 9);
    ChangeAndReload();
    EXPECT_EQ(HelloModule::Foo(4), -1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

要使得这个测试运行起来，还需要对 CMake 文件进行一些修改，这部分留作练习吧，动手试试会对 CMake 等有更深的理解。

相比较于 [projects/replex-3](https://github.com/zhangyi1357/Notes/tree/main/projects/replex-3)，需要修改的文件有：

1. 移除 main 文件夹
2. 根目录下的 CMakeLists.txt
3. hello/CMakeLists.txt
4. hello/include/hello.h
5. test/src/test.cpp

完整代码见 [projects/replex-4](https://github.com/zhangyi1357/Notes/tree/main/projects/replex-4)
