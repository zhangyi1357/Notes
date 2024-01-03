# C++ 动态库热加载

本文参考自 [project-based-learning](https://github.com/practical-tutorials/project-based-learning) 中的 [Build a Live Code-reloader Library for C++](https://howistart.org/posts/cpp/1/index.html)，主要内容都来自于其中，但是对代码进行了一点修改，并且改用 CMake 进行构建。

文章整体比较基础，适合初学者，通过本文可以学习到以下知识点

1. 关于 C++ 程序如何编译运行，如何运行时加载动态库（使用 `dl*` API）。
2. 如何使用 CMake 组织并构建一个包含可执行程序、动态库和头文件库的项目。

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
