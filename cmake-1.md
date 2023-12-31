# 现代 CMake 模块化项目管理指南

参考小彭老师的视频教程整理笔记，学习同时方便快速查阅，视频链接如下

[【公开课】现代 CMake 模块化项目管理指南【C/C++】](<https://www.bilibili.com/video/BV1V84y117YU?vd_source=07d6eec55261917555a5d7fb4429cab9>)

对应课程 PPT 和源码见

[https://github.com/parallel101/course](https://github.com/parallel101/course)

## 文件/目录组织规范

完整案例参考源码仓库
<https://github.com/parallel101/course/tree/master/16/00>

### 推荐的目录组织方式

```bash
.
├── biology
│   ├── CMakeLists.txt
│   ├── include
│   │   └── biology
│   │       └── Animal.h
│   └── src
│       └── Animal.cpp
├── CMakeLists.txt
└── pybmain
    ├── CMakeLists.txt
    ├── include
    │   └── pybmain
    │       └── myutils.h
    └── src
        └── main.cpp
```

第一点，划分了 biology 和 pybmain 两个子项目，每个子项目在各自目录下各有自己的 CMakeLists.txt 文件。

第二点，所有子项目中都被划分为了 include 和 src 两个子目录，分别用来放头文件和源码文件，而其中 include 目录又套了一层项目名，这样可以避免头文件名称冲突。子项目的 CMakeLists.txt 文件中需要使用

```cmake
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

源码文件中这样写，

```cpp
#include <biology/Animal.h>
#include <pybmain/myutils.h>
```

第三点，推荐每个模块都有自己的命名空间，头文件中需要

```cpp
#pragma once
namespace biology {
class Animal {
//...
};
}
```

源码文件中需要

```cpp
#include <biology/Animal.h>

namespace biology {
//...
};
```

### 根项目的 CMakeLists.txt 配置

```cmake
cmake_minimum_required(VERSION 3.15)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMake_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(CppCMakeDemo LANGUAGES CXX)

add_subdirectory(biology)
add_subdirectory(pybmain)
```

最后两行是关键，用来添加子项目，会调用子项目的 CMakeLists.txt 文件。

### 子项目的 CMakeLists.txt 配置

库文件的配置

```cmake
# biology/CMakeLists.txt
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(biology STATIC ${srcs})
target_include_directories(biology PUBLIC include)
```

首先使用 `GLOB_RECURSE` 命令获取所有源码文件，然后使用 `add_library` 命令添加静态库，最后使用 `target_include_directories` 命令添加头文件搜索路径。

1. `PUBLIC` 修饰符表示这个头文件搜索路径会被暴露给其他依赖这个库的项目，链接了 biology 库的 pybmain 项目也可以共享这个路径。
2. 注意到我们将 .h 文件也一并添加到了 `add_library` 命令中，这样可以确保 .h 文件也会被添加到 IDE 中，方便查看。
3. `GLOB_RECURSE` 相比 `GLOB` 允许匹配嵌套的目录。
4. `CONFIGURE_DEPENDS` 选项表示如果源码文件发生变化，`cmake --build build` 会检测目录是否更新，有新文件则自动重新运行 `cmake -B build` 重新配置项目。

可执行文件的配置

```cmake
# pybmain/CMakeLists.txt
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_executable(pybmain ${srcs})
target_include_directories(pybmain PUBLIC include)

target_link_libraries(pybmain PUBLIC biology)
```

基本和库文件的配置一致，只是使用 `add_executable` 命令添加可执行文件，使用 `target_link_libraries` 命令链接库文件。

### CMake 的 include 功能

和 C/C++ 的 #include 一样，CMake 也有一个 include 命令，CMake 会在 CMAKE_MODULE_PATH 中搜索相应的 XXX.cmake 文件。

```cmake
# ./CMakeLists.txt
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(CppCMakeDemo LANGUAGES CXX)

include(MyUsefulFuncs)
```

```cmake
# cmake/MyUsefulFuncs.cmake
macro (my_add_target name type)
    # 用法：my_add_target(pybmain EXECUTABLE)
    file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp src/*.h)
    if ("${type}" MATCHES "EXECUTABLE")
        add_executable(${name} ${srcs})
    else()
        add_library(${name} ${type} ${srcs})
    endif()
    target_include_directories(${name} PUBLIC include)
endmacro()

set(SOME_USEFUL_GLOBAL_VAR    ON)
set(ANOTHER_USEFUL_GLOBAL_VAR OFF)
```

macro 和 function 的区别

- macro 相当于直接把代码粘贴过去，直接访问调用者的作用域。这里写的相对路径 include 和 src，是基于调用者所在路径。
- function 则是会创建一个闭包，优先访问定义者的作用域。这里写的相对路径 include 和 src，则是基于定义者所在路径。

include 和 add_subdirectory 的区别

- include 相当于直接把代码粘贴过去，直接访问调用者的作用域。这里创建的变量和外面共享，直接 set(key val) 则调用者也有 ${key} 这个变量了。
- function 中则是基于定义者所在路径，优先访问定义者的作用域。这里需要 set(key val PARENT_SCOPE) 才能修改到外面的变量。

## 第三方库/依赖项配置

主要讲解 `find_package` 命令，其官方文档为
<https://cmake.org/cmake/help/latest/command/find_package.html>

### 用法举例

```cmake
# 查找名为 OpenCV 的包，找不到不报错，事后可以通过 ${OpenCV_FOUND} 查询是否找到。
find_package(OpenCV)
# 查找名为 OpenCV 的包，找不到不报错，也不打印任何信息。
find_package(OpenCV QUIET)
# 查找名为 OpenCV 的包，找不到就报错（并终止 cmake 进程，不再继续往下执行）。
find_package(OpenCV REQUIRED)    # 最常见用法
# 查找名为 OpenCV 的包，找不到就报错，且必须具有 OpenCV::core 和 OpenCV::videoio 这两个组件，如果没有这两个组件也会报错。
find_package(OpenCV REQUIRED COMPONENTS core videoio)
# 查找名为 OpenCV 的包，找不到就报错，可具有 OpenCV::core 和 OpenCV::videoio 这两个组件，没有这两组件不会报错，通过 ${OpenCV_core_FOUND} 查询是否找到 core 组件。
find_package(OpenCV REQUIRED OPTIONAL_COMPONENTS core videoio)
```

### `find_package` 原理

实际上 `find_package(OpenCV)` 是在找一个叫做 OpenCVConfig.cmake 的文件，这个文件是 OpenCV 项目提供的，用来告诉 cmake OpenCV 的安装路径和组件信息。

这个包配置文件由第三方库作者提供，在安装这个包时一并安装到系统中的，一般的约定是将其安装到 `/usr/lib/cmake/XXX/` 目录下，其中 XXX 为包名。

Windows 系统下的搜索路径

```bash
<prefix>/
<prefix>/cmake/
<prefix>/<name>*/
<prefix>/<name>*/cmake/
<prefix>/<name>*/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/cmake/
```

其中 `<prefix>` 是变量 `${CMAKE_PREFIX_PATH}`，Windows 平台默认为 `C:/Program Files`，`<name>` 是在 `find_package(<name> REQUIRED)` 命令中指定的包名，`<arch>` 是系统的架构名。

Unix 系统下的搜索路径

```bash
<prefix>/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/(lib/<arch>|lib*|share)/<name>*/
<prefix>/(lib/<arch>|lib*|share)/<name>*/cmake/
<prefix>/<name>*/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/cmake/
```

其中 `<prefix>` 是变量 `${CMAKE_PREFIX_PATH}`，Unix 平台默认为 `/usr`，`<name>` 是你在 `find_package(<name> REQUIRED)` 命令中指定的包名，`<arch>` 是系统的架构，例如 `x86_64-linux-gnu` 或 `i386-linux-gnu`（用于伺候 Ubuntu 喜欢把库文件套娃在 `/usr/lib/x86_64-linux-gnu` 目录下）。

另外 `<name>` 可以有额外后缀，例如 `find_package(Qt5)` 可以在 `Qt5.12.1/cmake` 或者 `Qt5xxx/cmake` 目录下找到 `Qt5Config.cmake` 文件。

### 非标准路径安装的库

想让 CMake 找到非标准路径安装的库，本质上是定义好 `NAME_DIR` 变量，告诉 CMake 库文件的路径，然后再调用 `find_package` 命令。

例如可以直接在 CMakeLists.txt 中定义该变量，

```cmake
# pybmain/CMakeLists.txt
set(OpenCV_DIR "/home/pyb/opencv/build")
find_package(OpenCV REQUIRED)
```

这种方式对该项目有效，但是不便于开源分发，因其路径直接写死在 CMakeLists.txt 中，其他人要用的时候就会找不到该路径下的 OpenCVConfig.cmake 文件。

也可以设置环境变量

```bash
export OpenCV_DIR="/home/pyb/opencv/build"
```

这种方式全局有效，但是不便于多版本共存，因为环境变量是全局的，如果要切换到其他版本的 OpenCV，就需要重新设置环境变量。

最好的方式是在调用 cmake 命令时定义该变量，例如

```bash
cmake -B build -DOpenCV_DIR="/home/pyb/opencv/build"
```

虽然每次需要在命令行中输入，但是这种方式既不会污染全局环境，也不会污染项目的 CMakeLists.txt，而且可以方便的切换版本。并且 CMake 本身有缓存功能，只要没有删除 build 目录下的 CMakeCache.txt 文件，下次再运行 `cmake -B build` 时不输入该变量 CMake 也会自动读取缓存中的值。

### 未提供 Config 文件的第三方库

有一些库非常热门，但是并未提供 Config 文件，例如 Python，CUDA，Jemalloc 等，这时候就需要我们自己写 FindXXX.cmake 文件来查找该库，幸运的是 CMake 已经为我们提供了这些库的 FindXXX.cmake 文件，可以在 CMake 安装目录下的 `share/cmake/Modules/` 目录下找到。

另外有一些库没有那么热门，CMake 也没有为我们提供 FindXXX.cmake 文件，这时候需要我们自己编写相应的 Find 文件，但是往往网上已经有人写过了，只需要搜索一下就可以找到，下面的链接是 Jemalloc 的 Find 文件

<https://github.com/AcademySoftwareFoundation/openvdb/blob/master/cmake/FindJemalloc.cmake>

这些文件有些使用的是古代 CMake 风格，有些是现代 CMake 风格，命名也不尽统一，但是一般都会有相应的说明文档，可以参考着使用。

### 指定 `find_package` 模式

`find_package` 命令有两种模式，一种是 `MODULE` 模式，一种是 `CONFIG` 模式。

- `MODULE` 模式，只会寻找 `FindTBB.cmake` 文件，而不会寻找 `TBBConfig.cmake` 文件，这种模式下，`find_package` 命令会在 `CMAKE_MODULE_PATH`（默认为 `/usr/share/cmake/Modules`）中搜索 `FindTBB.cmake` 文件

    ```cmake
    find_package(TBB MODULE REQUIRED)
    ```

- `CONFIG` 模式，只会寻找 `TBBConfig.cmake` 文件，而不会寻找 `FindTBB.cmake` 文件，这种模式下，`find_package` 命令会在 `${CMAKE_PREFIX_PATH}/lib/cmake/TBB`（默认为 `/usr/lib/cmake/TBB`）、`${TBB_DIR}` 和 `$ENV{TBB_DIR}` 中搜索 `TBBConfig.cmake` 文件

    ```cmake
    find_package(TBB CONFIG REQUIRED)
    ```

不指定则两者都会尝试，默认先查找 Find 文件，如果找不到再查找 Config 文件。

### 直接作为子模块引用

有些库并不是通过 `find_package` 命令来引用的，而是直接将其作为子模块引入项目中，例如

```cmake
add_subdirectory(spdlog)
target_link_libraries(myapp PUBLIC spdlog)
```
