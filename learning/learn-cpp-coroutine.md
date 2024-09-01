# C++ 协程学习

## 学习过程记录

1. [【\[C++\] 协程初步：手把手教你写一个协程】](https://www.bilibili.com/video/BV1c8411f7dw/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    20分钟的教程讲得非常清晰，适合入门。面向编译错误编程，逐步解决编译错误，最终实现一个猜数字游戏。

    完善版本的代码见 [projects/learn-cpp-coroutine/coroutine_basic.cpp](../projects/learn-cpp-coroutine/coroutine_basic.cpp)。

    视频中给协程画了一张图对比协程和普通函数：

    ![coroutine-vs-function](learn-cpp-coroutine/coroutine-vs-function.png)

2. [【\[C++\] 协程完结篇——实现思想】](https://www.bilibili.com/video/BV1U8411U7ud/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    基于[CppCon 2016: Gor Nishanov “C++ Coroutines: Under the covers"](https://www.youtube.com/watch?v=8C8NnE1Dg4A&t=8s)讲解了协程的底层实现原理，本质上就是一个状态机，在堆中存储运行过程所需的上下文，通过 `co_await`、`co_yield` 和 `co_return` 来控制状态的转移。


3. [【【协程革命】C++协程上手体验 干爽无废话 纯粹不卖课 全程字幕】](https://www.bilibili.com/video/BV1RV4y1L7ar/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    协程介绍，入门性质，主要是说未来是协程的。

4. [【【协程革命】理论篇！扫盲，辟谣一条龙！全语言通用，夯实基础，准备起飞！   全程字幕】](https://www.bilibili.com/video/BV1K14y1v7cw/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    协程理论，协程的定义（可以挂起和恢复的函数），协程的挂起和恢复是用户控制的，而普通函数是编译器控制的。还介绍了一些协程调度的内容。

5. [【【协程革命】实现篇！无栈协程 有手就行？！ 全程字幕】](https://www.bilibili.com/video/BV1dv4y127YT/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    使用switch 和 goto 语句以及一个 frame struct 模拟一个非对称协程斐波那契数列的实现，与2中的内容相似，但是更加简单易懂，适合入门。

    代码见 [projects/learn-cpp-coroutine/sim_coroutine_fib.cpp](../projects/learn-cpp-coroutine/sim_coroutine_fib.cpp)。

    协程的对称概念是关于其调度的一部分，C++所采用的对称协程可由用户自定义当前协程暂停之后恢复执行的协程，而非对称协程则只能恢复到调度器。

6. [cpp reference 协程](https://en.cppreference.com/w/cpp/language/coroutines)

    官方文档，稍显晦涩，可以用作手册查阅。