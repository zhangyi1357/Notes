# CMake 学习笔记

CMake 已经是 C++ 构建系统的事实标准。

主要是对小彭老师的 C++ 视频课程中 CMake 相关部分的一些笔记和整理，视频链接如下

1. [学 C++ 从 CMake 学起](https://www.bilibili.com/video/BV1fa411r7zp?vd_source=07d6eec55261917555a5d7fb4429cab9)
2. [现代 CMake 高级教程](https://www.bilibili.com/video/BV16P4y1g7MH?vd_source=07d6eec55261917555a5d7fb4429cab9)

包含视频中的代码和 PPT 的仓库见以下链接

[https://github.com/parallel101/course](https://github.com/parallel101/course)

本笔记重点关注与 CMake 相关的一些知识点，需要的前置知识为 C++ 本身的头文件机制、编译流程、Makefile 的基本认知等内容，所以不会赘述课程中出现的一些很基本的内容。

[toc]

## 学 C++ 从 CMake 学起

### 基本的 C++ 编译相关的命令

编译单文件为可执行文件

```bash
g++/clang++ main.cpp -o main.out
```

查看 binary 文件的反汇编代码

```bash
objdump -D binary | less
```

查看 binary 文件的共享库依赖

```bash
ldd binary
```

### CMake 简介

1. 构建系统的构建系统。
2. 跨平台，只需要一份 CMakeLists.txt 文件就可以在不同的平台上使用相应的构建系统来构建项目。
3. 自动检测源文件和头文件之间的依赖关系，导出到 Makefile 里。
4. 自动检测编译器，使用对应的 flag。

### 静态库和动态库

![静态库和动态库](figs/cmake-image.png)

1. 静态库相当于直接把代码插入到生成的可执行文件中，会导致体积变大，但是只需要一个文件即可运行
2. 动态库则只在生成的可执行文件中生成“插桩”函数，当可执行文件被加载时会读取指定目录中的 .dll 文件，加载到内存中空闲的位置，并且替换相应的“插桩”指向的地址为加载后的地址，这个过程称为重定向。这样以后函数被调用就会跳转到动态加载的地址去。

### CMake 中的静态库和动态库

使用 `add_library` 生成库文件

```cmake
add_library(test STATIC source1.cpp source2.cpp)  # 生成静态库 libtest.a
add_library(test SHARED source1.cpp source2.cpp)  # 生成动态库 libtest.so
```

创建库之后，要在某个可执行文件中使用该库，只需要：

```cmake
target_link_libraries(myexec PUBLIC test)
```

### CMake 中的子模块

在根目录的 CMakeLists.txt 文件中使用 add_subdirectory 添加子目录，然后在子目录中也写一个 CMakeLists.txt，然后在其中定义库，所有的子模块都可以使用这个库。目录结构如下

```bash
project
├── CMakeLists.txt
├── hellolib
│   ├── CMakeLists.txt
│   ├── hello.cpp
│   └── hello.h
└── main.cpp
```

其中根目录下的 CMakeLists.txt 文件如下

```cmake
cmake_minimum_required(VERSION 3.12)
project(hellocmake LANGUAGES CXX)

add_subdirectory(hellolib)

add_executable(a.out main.cpp)
target_link_libraries(a.out PUBLIC hellolib)
```

hellolib 子目录下的 CMakeLists.txt 文件如下

```cmake
add_library(hellolib STATIC hello.cpp)
target_include_directories(hellolib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) # 添加当前目录到 hellolib 的头文件搜索路径中， PUBLIC 表示传播给 hellolib 的用户
```

其中 `target_include_directories` 的作用是将当前目录添加到 hellolib 库的头文件搜索路径中，这样在 hellolib 库中的头文件就可以直接使用 `#include "hello.h"` 的方式来引用了，甚至可以用 `#include <hello.h>` 的方式来引用，因为 CMake 会自动将当前目录添加到 hellolib 库的头文件搜索路径中。

而其中 `PUBLIC` 的作用是将这个属性传播给 hellolib 库的用户，这样 hellolib 的用户也可以直接使用 `#include "hello.h"` 的方式来引用了，例如在根目录下的 main.cpp 中可以直接使用 `#include "hello.h"` 的方式来引用。

如果不想让 hellolib 的用户自动添加这个头文件搜索路径，可以使用 `PRIVATE` 属性，这样只有 hellolib 库内部才能使用 `#include "hello.h"` 的方式来引用。

### CMake 中其他目标选项

```cmake
target_include_directories(myapp PUBLIC /usr/include/eigen3)  # 添加头文件搜索目录
target_link_libraries(myapp PUBLIC hellolib)                  # 添加要链接的库
target_add_definitions(myapp PUBLIC MY_MACRO=1)               # 添加一个宏定义
target_add_definitions(myapp PUBLIC -DMY_MACRO=1)             # 与 MY_MACRO=1 等价
target_compile_options(myapp PUBLIC -fopenmp)                 # 添加编译器命令行选项
target_sources(myapp PUBLIC hello.cpp other.cpp)              # 添加要编译的源文件
```

现在已经不推荐使用不针对特定目标的命令了，例如 `add_definitions`、`include_directories`、`link_libraries` 等，而是使用 `target_xxx` 的方式来添加属性，这样可以针对特定的目标添加属性，而不是添加全局属性。

### 第三方库

#### 纯头文件引入

这里是一些好用的 header-only 库：

- nothings/stb - 大名鼎鼎的 stb_image 系列，涵盖图像，声音，字体等，只需单头文件！
- Neargye/magic_enum - 枚举类型的反射，如枚举转字符串等（实现方式很巧妙）
- g-truc/glm - 模仿 GLSL 语法的数学矢量/矩阵库（附带一些常用函数，随机数生成等）
- Tencent/rapidjson - 单纯的 JSON 库，甚至没依赖 STL（可定制性高，工程美学经典）
- ericniebler/range-v3 - C++20 ranges 库就是受到他启发（完全是头文件组成）
- fmtlib/fmt - 格式化库，提供 std::format 的替代品（需要 -DFMT_HEADER_ONLY）
- gabime/spdlog - 能适配控制台，安卓等多后端的日志库（和 fmt 冲突！）

优点：简单方便，只需要把他们的 include 目录或头文件下载下来，然后 `include_directories(spdlog/include)` 即可。

缺点：函数直接实现在头文件里，没有提前编译，从而需要重复编译同样内容，编译时间长。

#### 子模块引入

直接把相应的库放到工程的根目录，然后 `add_subdirectory` 即可。

这些库能够很好地支持作为子模块引入：

- fmtlib/fmt - 格式化库，提供 std::format 的替代品
- gabime/spdlog - 能适配控制台，安卓等多后端的日志库
- ericniebler/range-v3 - C++20 ranges 库就是受到他启发
- g-truc/glm - 模仿 GLSL 语法的数学矢量/矩阵库
- abseil/abseil-cpp - 旨在补充标准库没有的常用功能
- bombela/backward-cpp - 实现了 C++ 的堆栈回溯便于调试
- google/googletest - 谷歌单元测试框架
- google/benchmark - 谷歌性能评估框架
- glfw/glfw - OpenGL 窗口和上下文管理
- libigl/libigl - 各种图形学算法大合集

#### 引用系统中预安装的库

可以通过 find_package 命令寻找系统中的包/库：

```cmake
find_package(fmt REQUIRED)
target_link_libraries(myexec PUBLIC fmt::fmt)
```

值得注意的是，这里不是简单的 fmt 而是 fmt::fmt，这是因为现代 CMake 认为一个包 (package) 可以提供多个库，又称组件 (components)，比如 TBB 这个包，就包含了 tbb, tbbmalloc, tbbmalloc_proxy 这三个组件。

可以指定使用哪些组件，如下

```cmake
find_package(TBB REQUIRED COMPONENTS tbb tbbmalloc REQUIRED)
target_link_libraries(myexec PUBLIC TBB::tbb TBB::tbbmalloc)
```

#### 包管理器

Linux 可以用系统自带的包管理器（如 apt）安装 C++ 包，例如

```bash
sudo apt install libfmt-dev
```

Windows 则没有自带的包管理器。因此可以用跨平台的 vcpkg：<https://github.com/microsoft/vcpkg>

```bash
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install fmt:x64-windows
cd ..
cmake -B build -DCMAKE_TOOLCHAIN_FILE="%CD%/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

## 现代 CMake 高级教程

现代 CMake 主要指 3.x 版本的 CMake，提供了更加方便的命令行指令和更加清晰简洁的语法。

### 命令行小技巧

#### `-B` 和 `--build` 选项

现代 CMake 提供了跨平台的 `-B` 和 `--build` 指令，更加方便好用。

```bash
cmake -B build
cmake --build build -j4
cmake --build build --target install
```

CMake 项目的构建分为两步，

1. 第一步是 `cmake -B build`，称为配置阶段（configure），这时只检测环境并生成构建规则
会在 build 目录下生成本地构建系统能识别的项目文件（Makefile 或是 .sln）
2. 第二步是 `cmake --build build`，称为构建阶段（build），这时才实际调用编译器来编译代码

#### `-D` 选项

在配置阶段可以通过 `-D` 设置缓存变量。第二次配置时，之前的 `-D` 添加仍然会被保留。

```bash
# 设置安装路径为 /opt/openvdb-8.0（会安装到 /opt/openvdb-8.0/lib/libopenvdb.so）
cmake -B build -DCMAKE_INSTALL_PREFIX=/opt/openvdb-8.0
# 设置构建模式为发布模式（开启全部优化）
cmake -B build -DCMAKE_BUILD_TYPE=Release
# 第二次配置时没有 -D 参数，但是之前的 -D 设置的变量都会被保留
#（此时缓存 CMakeCache.txt 文件里仍有之前定义的 CMAKE_BUILD_TYPE 和 CMAKE_INSTALL_PREFIX）
cmake -B build
```

#### `-G` 选项

Linux 系统上的 CMake 默认用是 Unix Makefiles 生成器；Windows 系统默认是 Visual Studio 2019 生成器；MacOS 系统默认是 Xcode 生成器。

可以用 -G 参数改用别的生成器，例如 `cmake -G Ninja` 会生成 Ninja 这个构建系统的构建规则。

Ninja 是一个高性能，跨平台的构建系统，Linux、Windows、MacOS 上都可以用。
Ninja 是专为性能优化的构建系统（比 MSbuild 和 Makefile 效率要高），他和 CMake 结合都是行业标准了，推荐使用。

### 添加源文件

1. 直接在 `add_executable` 中添加源文件

    ```cmake
    add_executable(myapp main.cpp hello.cpp)
    ```

2. 先创建目标，然后添加源文件

    ```cmake
    add_executable(myapp)
    target_sources(myapp PUBLIC main.cpp hello.cpp)
    ```

3. 使用 GLOB 自动查找源文件

    ```cmake
    file(GLOB sources *.cpp *.h)
    add_executable(myapp ${sources})
    ```

    这种方法有一些缺点，比如如果添加了新的源文件，需要重新运行 CMake 才能生效。
    可以使用 CONFIGURE_DEPENDS 选项来解决这个问题，但是这个选项只有在 CMake 3.12 以上才支持。

    ```cmake
    file(GLOB sources CONFIGURE_DEPENDS *.cpp *.h)
    add_executable(myapp ${sources})
    ```

    另外，现在这样没法递归搜索子目录，如果要递归搜索子目录，可以使用 `file(GLOB_RECURSE)` 命令。

    ```cmake
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.cpp *.h)
    add_executable(myapp ${sources})
    ```

4. 使用 `aux_source_directory` 命令，可以自动搜集需要的文件后缀名

    ```cmake
    aux_source_directory(. sources)
    add_executable(myapp ${sources})
    ```

    这种方法和 GLOB 类似，也有一些缺点，比如如果添加了新的源文件，需要重新运行 CMake 才能生效。

    另外，这种方法也没法递归搜索子目录，需要自己指定子目录

    ```cmake
    aux_source_directory(. sources)
    aux_source_directory(./subdir sources)
    add_executable(myapp ${sources})
    ```

值得注意的一点是 .h 文件不添加到源文件中仍然可以正常编译运行，但是添加之后可以使得 IDE 中可以显示文件，因此建议是将 .h 文件也添加到源文件中。

另外 GLOB_RECURSE 会把 build 目录里生成的临时 .cpp 文件也加进来，一般建议将源码统一放到 src 目录中。

### 项目变量配置

#### 构建模式 `CMAKE_BUILD_TYPE`

Release、Debug、MinSizeRel、RelWithDebInfo 是 CMake 内置的构建模式，可以通过 `cmake -B build -DCMAKE_BUILD_TYPE=Release` 来指定构建模式，其具体含义如下

| Build Mode      | Compiler Flags       | Meaning               |
|-----------------|----------------------|-----------------------|
| Release         | -O3 -DNDEBUG         | Optimized, no debug   |
| Debug           | -O0 -g               | Debug symbols enabled |
| MinSizeRel      | -Os -DNDEBUG         | Optimized, minimal size |
| RelWithDebInfo  | -O2 -g -DNDEBUG      | Optimized, with debug symbols |

NDEBUG 宏会移除代码中的 assert 语句。

CMake 中 `CMAKE_BUILD_TYPE` 的默认值为 `""`，即不指定构建模式，这时默认使用 Debug 模式。如果想设置默认模式为 Release 模式，可以在 CMakeLists.txt 中添加如下代码

```cmake
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
```

#### `project` 函数相关变量

CMake 中的 `project` 函数会定义一些变量，这些变量可以在 CMakeLists.txt 中使用，也可以在 C++ 代码中使用。

```cmake
project(myproject VERSION 1.0.0 LANGUAGES CXX)
message("PROJECT_NAME: ${PROJECT_NAME}")
message("PROJECT_VERSION: ${PROJECT_VERSION}")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message("myproject_SOURCE_DIR: ${myproject_SOURCE_DIR}")
message("myproject_SOURCE_DIR: ${${PROJECT_NAME}_SOURCE_DIR}") # 与上面一样，展示了嵌套 $ 功能
```

`PROJECT_SOURCE_DIR` 代表最近一次调用 `project` 语句的 CMakeLists.txt 的目录

`CMAKE_CURRENT_SOURCE_DIR` 代表当前 CMakelists.txt 所在的目录。

`CMAKE_SOURCE_DIR` 代表顶层 CMakeLists.txt 所在的目录。（不建议使用，无法作为子项目使用）

更多变量和内容可以查看 `project` 语句的官方文档
<https://cmake.org/cmake/help/latest/command/project.html>

#### C++ 标准变量 `CMAKE_CXX_STANDARD`

```cmake
set(CMAKE_CXX_STANDARD 17)          # 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 必须使用指定的标准
set(CMAKE_CXX_EXTENSIONS ON)        # 启用 GCC 特有的一些扩展功能

project(myproject VERSION 1.0.0 LANGUAGES CXX)
```

上面几条命令比较容易理解，值得注意的一点是 `project` 函数放在设置之后，这样 CMake 可以在 project 函数里对编译器进行一些检测，看看他能不能支持 C++17 的特性。

### 标准 C++ 项目模板

```cmake
cmake_minimum_required(VERSION 3.15)

set(CMake_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(myproject LANGUAGES CXX)

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(WARNING "In-source build detected! Please build out-of-source!")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (WIN32)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

if (NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)
    if (CCACHE_PROGRAM)
        message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
    endif()
endif()
```

### 链接库文件

#### 对象库

参考链接<https://www.scivision.dev/cmake-object-libraries>

对象库是 CMake 自创的，绕开了编译器和操作系统的各种繁琐规则，保证了跨平台统一性，类似于静态库，但不生成 .a 文件，只由 CMake 本身记住该库生成了哪些对象文件。在自己的项目中，推荐全部用对象库来组织代码。

```cmake
add_library(mylib OBJECT lib.cpp)
add_executable(myapp main.cpp $<TARGET_OBJECTS:mylib>)
```

另外使用静态库时 GCC 编译器会自动剔除没有引用符号的对象，如果使用静态库下面的程序会仅输出 world 而没有 hello，对象库则不会有这种问题。

```cpp
// lib.cpp
#include <cstdio>
static int unused = printf("hello\n");

// main.cpp
#include <cstdio>
int main() {
    printf("world\n");
}
```

#### `add_library` 的默认参数

当不填写 `add_library` 的静态库/动态库参数时，CMake 会根据 BUILD_SHARED_LIBS 变量来决定是生成静态库还是动态库，未指定 BUILD_SHARED_LIBS 时默认生成静态库。

可以通过命令行参数或者 CMake 语句来指定 BUILD_SHARED_LIBS 变量：

```bash
cmake -B build -DBUILD_SHARED_LIBS:BOOL=ON
```

```cmake
if (NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
endif()
```

#### 动态库无法链接静态库

当我们要编译一个 so 提供给外部使用，这个 so 本身依赖一些第三方库。但是我们却希望 so 的使用者不用关心该 so 对其他库的依赖。很自然的是会想到在编译 so 的时候把依赖到的第三方库静态链接进来。

然而静态库中的代码位置都是确定的，而动态库中的代码位置是不确定的，因此动态库无法链接静态库。通过将静态库也编译成位置无关的代码（Position Independent Code，PIC），就可以解决这个问题，实现这一点有两种方式，一种是设置全局变量，另一种是设置目标变量。

```cmake
# 设置全局变量
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(mylib STATIC lib.cpp)
add_library(mylib_shared SHARED lib.cpp)
target_link_libraries(mylib_shared PRIVATE mylib)

# 设置目标变量
add_library(mylib STATIC lib.cpp)
set_target_properties(mylib_shared PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_library(mylib_shared SHARED lib.cpp)
target_link_libraries(mylib_shared PRIVATE mylib)
```

#### Windows 下的动态链接库

需要在需要导出的内容前面加上 `__declspec(dllexport)`，在需要导入的内容前面加上 `__declspec(dllimport)`。

```cpp
// lib.h
# pragma once
#ifdef _MSC_VER
    #ifdef BUILD_SHARED_LIBS
        #define LIB_API __declspec(dllexport)
    #else
        #define LIB_API __declspec(dllimport)
    #endif
#else
    #define LIB_API
#endif

LIB_API void hello();

// lib.cpp
#include "lib.h"
#include <cstdio>
LIB_API void hello() { printf("hello\n"); }
```

另外由于 Windows 不支持 [RPATH](https://en.wikipedia.org/wiki/Rpath)，因此动态库的 dll 文件需要放在可执行文件的同一目录下，或者放在系统目录下，在子模块的情况下就需要将子模块的输出目录设置为可执行文件的同一目录下。

```cmake
add_library(mylib SHARED lib.cpp)
set_target_properties(TARGET mylib PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})
set_target_properties(TARGET mylib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR})
```

### 对象的属性

`set_property` 命令可以设置对象的属性，例如

```cmake
add_executable(myapp main.cpp)

set_property(TARGET myapp PROPERTY CXX_STANDARD 17)          # 采用 C++ 标准为 C++17 （默认 11）
set_property(TARGET myapp PROPERTY CXX_STANDARD_REQUIRED ON) # 必须使用指定的标准 （默认 OFF）
set_property(TARGET myapp PROPERTY WIN32_EXECUTABLE ON)      # 生成 Windows 窗口程序，不启动控制台，只有 GUI 界面（默认 OFF）
set_property(TARGET myapp PROPERTY LINK_WHAT_YOU_USE ON)     # 告诉编译器不要自动剔除没有使用的符号（默认 OFF）
set_property(TARGET myapp PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 设置动态库文件输出目录
set_property(TARGET myapp PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 设置静态库文件输出目录
set_property(TARGET myapp PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 设置可执行文件输出目录
```

也可以使用 `set_target_properties` 命令来一次设置多条对象的属性，例如

```cmake
add_executable(myapp main.cpp)

set_target_properties(myapp PROPERTIES
    CXX_STANDARD 17          # 采用 C++ 标准为 C++17 （默认 11）
    CXX_STANDARD_REQUIRED ON # 必须使用指定的标准 （默认 OFF）
    WIN32_EXECUTABLE ON      # 生成 Windows 窗口程序，不启动控制台，只有 GUI 界面（默认 OFF）
    LINK_WHAT_YOU_USE ON     # 告诉编译器不要自动剔除没有使用的符号（默认 OFF）
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib # 设置动态库文件输出目录
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib # 设置静态库文件输出目录
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin # 设置可执行文件输出目录
)
```

也可以设置全局的属性

```cmake
set(CMAKE_CXX_STANDARD 17)          # 设置 C++ 标准为 C++17 （默认 11）
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 必须使用指定的标准 （默认 OFF）
set(CMAKE_WIN32_EXECUTABLE ON)      # 生成 Windows 窗口程序，不启动控制台，只有 GUI 界面（默认 OFF）
set(CMAKE_LINK_WHAT_YOU_USE ON)     # 告诉编译器不要自动剔除没有使用的符号（默认 OFF）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 设置动态库文件输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 设置静态库文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 设置可执行文件输出目录

add_executable(myapp main.cpp)
```

### 链接第三方库

#### 以链接 tbb 为例

例如需要链接 tbb 库，直接链接的话 CMake 会在系统的库目录中查找 tbb，但是在 Windows 上没有系统的库目录，因此需要指定 tbb 的位置，而且头文件也需要指定，非常麻烦。

有一个优雅的做法是使用现代的 `find_package` 命令，这个命令会查找 /usr/lib/cmake/TBB/TBBConfig.cmake 这个配置文件（地址不一定一样，大概就是查找库的文件的目录），并根据里面的配置信息创建 TBB::tbb 这个伪对象（他实际指向真正的 tbb 库文件路径 /usr/lib/libtbb.so）。

```cmake
find_package(TBB CONFIG REQUIRED)
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE TBB::tbb)
```

其中 CONFIG 的作用是只查找 TBBConfig.cmake 这个配置文件，不查找 FindTBB.cmake 这个脚本文件（项目作者常把他塞在 cmake/ 目录里并添加到 `CMAKE_MODULE_PATH`），建议加，加上能保证项目寻找包的 .cmake 脚本与系统自带的版本是适配的，而不是项目作者当年下载的版本的 .cmake 脚本。

- 不加 CONFIG：会先查找 TBBConfig.cmake，如果找不到再查找 FindTBB.cmake
- 加了 CONFIG：只查找 TBBConfig.cmake，不查找 FindTBB.cmake

这个配置文件一般由库的作者提供，通过包管理器安装包后也会自动安装这个文件，其内容大概如下

```cmake
# Create imported target TBB::tbb
add_library(TBB::tbb SHARED IMPORTED)

set_target_properties(TBB::tbb PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "\$<\$<CONFIG:DEBUG>:TBB_USE_DEBUG>"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
)

# Create imported target TBB::tbbmalloc
add_library(TBB::tbbmalloc SHARED IMPORTED)

set_target_properties(TBB::tbbmalloc PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "\$<\$<CONFIG:DEBUG>:TBB_USE_DEBUG>"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
)

# Create imported target TBB::tbbmalloc_proxy
add_library(TBB::tbbmalloc_proxy SHARED IMPORTED)

set_target_properties(TBB::tbbmalloc_proxy PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "\$<\$<CONFIG:DEBUG>:TBB_USE_DEBUG>"
  INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include"
)
```

#### Windows 使用 `find_package` 查找第三方库

Windows 上并没有库文件目录，因此需要手动指定 TBBConfig.cmake 文件的位置，有许多方式可以做到这一点，例如设置 `CMAKE_MODULE_PATH` 变量，也可以设置 `TBB_DIR` 变量，可以在 CMakeLists.txt 中设置，也可以在命令行中设置（推荐，因为是用户相关的内容，每个人的安装路径都不一样），例如

```bash
cmake -B build -DTBB_DIR="C:/Program Files/TBB/cmake"
```

#### 链接 Qt5

直接用以下方式链接 Qt5 会报错

```cmake
find_package(Qt5 REQUIRED)
add_executable(myapp main.cpp)
```

报错信息为

```txt
CMake Error at CMakeLists.txt:6 (find_package):
    Found package configuration file:

        /usr/lib/cmake/Qt5/Qt5Config.cmake

    but it set Qt5_FOUND to FALSE so package "Qt5" is considered to be NOT
    FOUND.  Reason given by package:

    The Qt5 package requires at least one component
```

原因是 Qt5 具有多个组件，需要指定链接哪些组件，`find_package` 生成的伪对象 (imported target) 都按照“包名：: 组件名”的格式命名。例如

```cmake
find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)
add_executable(myapp main.cpp)
target_link_libraries(myapp PRIVATE Qt5::Widgets Qt5::Gui)
```

#### 可选依赖

如果希望某个库是可选的，可以不使用 `find_package` 的 REQUIRED 参数，然后如下定义相应的宏

```cmake
find_package(TBB CONFIG)
if (TBB_FOUND) # 也可以用 if (TARGET TBB::tbb)
    message(STATUS "TBB found")
    add_executable(myapp main.cpp)
    target_link_libraries(myapp PRIVATE TBB::tbb)
    target_compile_definitions(myapp PRIVATE WITH_TBB)
else()
    message(WARNING "TBB not found! using serial version")
```

于是在 cpp 文件中可以这样写

```cpp
#include <cstdio>
#ifdef WITH_TBB
#include <tbb/parallel_for.h>
#endif

int main() {
#ifdef WITH_TBB
    tbb::parallel_for(0, 100, [](int i) { printf("%d\n", i); });
#else
    for (int i = 0; i < 100; i++) printf("%d\n", i);
#endif
```

### 输出和变量

`message` 命令可以输出信息，例如

```cmake
message(STATUS "STATUS means status info with -- prefix")
message(WARNING "WARNING means warning info")
message(AUTHOR_WARNING "AUTHOR_WARNING is only useful for project authors and can be closed by -Wno-dev")
message(SEND_ERROR "SEND_ERROR means error info and continue to run")
message(FATAL_ERROR "FATAL_ERROR means error info and stop running")
```

`message` 也可以打印变量的值，例如

```cmake
set(myvar "hello world")
message(STATUS "myvar = ${myvar}") # result is "myvar = hello world"

set(myvar hello world)
message(STATUS "myvar = ${myvar}") # result is "myvar = hello;world"
```

### 变量与缓存

#### CMake 缓存

CMake 会自动将一些编译器和 C++ 特性等内容检测完后缓存到 CMakeCache.txt 文件中，这样下次运行 CMake 时就不用再检测了，直接读取缓存文件即可，这样可以加快 CMake 的运行速度。

前面用到的 `find_package` 命令就是一个例子，他会将检测到的库的路径缓存到 CMakeCache.txt 文件中，这样下次运行 CMake 时就不用再检测了。

缓存虽好，但是很多时候情况有变需要更新缓存，会导致很多 CMake 出错的情况，这时很多人会经典的删 build 大法，但是这样需要完全重新构建，非常耗时，可以尝试只删除 CMakeCache.txt 文件，然后重新运行 `CMake -B build`，这样就可以更新缓存了。

#### 缓存变量

可以用如下方式设置缓存变量

```cmake
set(myvar "hello world" CACHE STRING "this is the docstring of myvar") # The last string is a docstring
message(STATUS "myvar = ${myvar}") # result is "myvar = hello world"
```

这里有一个坑点，更新 CMakeCache.txt 文件后，CMakeLists.txt 中的变量并不会自动更新，需要重新运行 CMake 才会更新，这时可以使用 `FORCE` 参数来强制更新变量

```cmake
sett(myvar "hello world" CACHE STRING "this is the docstring of myvar" FORCE) # The last string is a docstring
message(STATUS "myvar = ${myvar}") # result is "myvar = hello world"
```

其实也可以通过命令行参数来更新

```bash
cmake -B build -Dmyvar="goodbye world"
```

另外还可以通过图形界面来编辑缓存变量，Linux 上可以使用 `ccmake` 命令，Windows 上可以使用 `cmake-gui` 命令。

最后还可以通过直接编辑 CMakeCache.txt 文件来更新缓存变量，该文件被设置为文本文件就是可供用户手工编辑或者被第三方软件打开并解析的。

#### 缓存变量类型

缓存变量由如下类型

- STRING 字符串，例如 “hello, world”
- FILEPATH 文件路径，例如 “C:/vcpkg/scripts/buildsystems/vcpkg.cmake”
- PATH 目录路径，例如 “C:/Qt/Qt5.14.2/msvc2019_64/lib/cmake/”
- BOOL 布尔值，只有两个取值：ON 或 OFF。
注意：TRUE 和 ON 等价，FALSE 和 OFF 等价；YES 和 ON 等价，NO 和 OFF 等价

CMake 对 BOOL 类型的缓存变量的 set 指令提供了一个简写 option，例如

```cmake
add_executable(myapp main.cpp)

option(WITH_TBB "set to ON to enable TBB, OFF disale TBB" ON)
if (WITH_TBB)
    find_package(TBB CONFIG REQUIRED)
    target_link_libraries(myapp PRIVATE TBB::tbb)
    target_compile_definitions(myapp PRIVATE WITH_TBB)
endif()
```

跟前面一样，容易犯的一个经典错误就是直接改 CMakeLists.txt 文件，然后重新运行 CMake，这样是不会更新缓存变量的。官方推荐做法是使用 `-DTBB:BOOL=ON/OFF` 命令行参数来更新缓存变量。

#### 绕开缓存

既然缓存有这么多坑，那么我们就可以绕开缓存，直接使用带普通变量的默认值来达到相同的效果，例如

```cmake
if (NOT DEFINED WITH_TBB)
    set(WITH_TBB ON)
endif()
```

一般来讲，CMake 自带的变量都会用这个方式来设置，但是就是会在 ccmake 中看不到。

### 跨平台与编译器

#### 宏变量跨平台

可以根据操作系统不同定义不同的宏变量，例如

```cmake
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(main PUBLIC MY_NAME="Bill Gates")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(main PUBLIC MY_NAME="Linus Torvalds")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(main PUBLIC MY_NAME="Steve Jobs")
endif()
```

这样代码中就可以

```cpp
#include <cstdio>
int main() {
#ifdef MY_NAME
    printf("Hello, %s!\n", MY_NAME);
#else
    printf("Hello, world!\n");
#endif
}
```

前面的 CMake 代码使用简写变量可读性会稍微强一点

```cmake
if (WIN32)
    target_compile_definitions(main PUBLIC MY_NAME="Bill Gates")
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(main PUBLIC MY_NAME="Linus Torvalds")
elseif (APPLE)
    target_compile_definitions(main PUBLIC MY_NAME="Steve Jobs")
endif()
```

还可以使用 [生成器表达式](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:PLATFORM_ID
) 简化为一条语句

```cmake
target_compile_definitions(main PUBLIC
    $<$<PLATFORM_ID:Windows>:MY_NAME="Bill Gates">
    $<$<PLATFORM_ID:Linux>:MY_NAME="Linus Torvalds">
    $<$<PLATFORM_ID:Darwin>:MY_NAME="Steve Jobs">)

# using comma to separate
target_compile_definitions(main PUBLIC
    $<$<PLATFORM_ID:Windows>:MY_NAME="DOS-like">,
    $<$<PLATFORM_ID:Linux,Darwin,FreeBSD>:MY_NAME="Unix-like">
```

#### 编译器

[判断所使用的编译器](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html#variable:CMAKE_%3CLANG%3E_COMPILER_ID)，如下

```cmake
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "using gcc")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "using MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "using clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "NVIDIA")
    message(STATUS "using nvcc")
endif()
```

从命令行指定编译器

```bash
cmake -B build -DCMAKE_CXX_COMPILER="/usr/bin/clang++"
```

也可以通过环境变量指定编译器

```bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake -B build
```

### 条件与判断

if 语句中的变量会自动展开，无需添加 `${}`，例如

```cmake
set(myvar "hello world")
if (myvar STREQUAL "hello world")
    message(STATUS "myvar is hello world")
endif()
```

如果要加 `${}`，则需要用引号括起来（否则因为其特殊的规则会有一些其奇奇怪怪的问题），例如

```cmake
set(myvar "hello world")
if ("${myvar}" STREQUAL "hello world")
    message(STATUS "myvar is hello world")
endif()
```

### 变量与作用域

#### 变量的传播

变量的传播规则：父会传递给子，子不会传递给父，兄弟之间不会传递。

如果子模块需要向父模块传递变量，可以使用 `set` 命令的 `PARENT_SCOPE` 参数，例如

```cmake
set(myvar "hello world" PARENT_SCOPE)
```

缓存变量全局可见，会传播到整个项目中。

#### 独立作用域

- include 的 XXX.cmake 没有独立作用域
- add_subdirectory 的 CMakeLists.txt 有独立作用域
- macro 没有独立作用域
- function 有独立作用域（因此 PARENT_SCORE 也可以用于 function 的返回值）

更多内容可以参考以下链接
<https://cmake.org/cmake/help/v3.16/command/set.html>
<https://blog.csdn.net/Calvin_zhou/article/details/104060927>

#### 变量的访问

用 `${xx}` 访问的是局部变量，局部变量服从刚刚所说的父子模块传播规则。

用 `$ENV{xx}` 访问的是环境变量，环境变量是全局的，不服从父子模块传播规则。

```cmake
message(STATUS "PATH = $ENV{PATH}")
```

用 `$CACHE{xx}` 访问的是缓存变量，缓存变量是全局的，不服从父子模块传播规则。

```cmake
message(STATUS "CMAKE_CXX_COMPILER_ID = $CACHE{CMAKE_CXX_COMPILER_ID}")
```

`${XX}` 找不到局部变量时会去找缓存变量，如果缓存变量也找不到，就会报错。

`if (DEFINED myvar)` 会判断 myvar 是否被定义，而 `if (myvar)` 会判断 myvar 的值是否为空，前者即使 myvar 的值为空也会返回 true。

`if (DEFINED ENV{XX})` 判断环境变量是否被定义，`if (DEFINED CACHE{XX})` 判断缓存变量是否被定义。

### 其他小建议

#### CCache：编译加速缓存

CCache 官方网站：<https://ccache.dev/>

将所有的编译链接指令前加上 `ccache` 即可，例如

```cmake
cmake_minimum_required(VERSION 3.15)
project(hellocmake)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Found ccache: ${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()
```

#### 添加 run 伪目标

创建一个 run 伪目标，执行 main 的可执行文件，例如

```cmake
add_executable(main main.cpp)

add_custom_target(run
    COMMAND $<TARGET_FILE:main>
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
```

然后就可以通过以下命令运行程序了

```bash
cmake --build build --target run
```

其最大的好处是跨平台，不用考虑在 Windows 上使用 build\main.exe 还是在 Linux 上使用 build/main，CMake 会自动处理这个问题。

#### 添加 configure 伪目标

创建一个 configure 伪目标，可视化修改缓存，好处同样是跨平台。

```cmake
if (CMAKE_EDIT_COMMAND)
    add_custom_target(configure
        COMMAND ${CMAKE_EDIT_COMMAND}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
```
