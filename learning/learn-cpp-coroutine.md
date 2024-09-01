# C++ 协程学习

## 学习过程记录

1. [【\[C++\] 协程初步：手把手教你写一个协程】](https://www.bilibili.com/video/BV1c8411f7dw/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    20 分钟的教程讲得非常清晰，适合入门。面向编译错误编程，逐步解决编译错误，最终实现一个猜数字游戏。

    完善版本的代码见 [projects/learn-cpp-coroutine/coroutine_basic.cpp](../projects/learn-cpp-coroutine/coroutine_basic.cpp)。

    视频中给协程画了一张图对比协程和普通函数：

    ![coroutine-vs-function](learn-cpp-coroutine/coroutine-vs-function.png)

2. [【\[C++\] 协程完结篇——实现思想】](https://www.bilibili.com/video/BV1U8411U7ud/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    基于 [CppCon 2016: Gor Nishanov “C++ Coroutines: Under the covers"](https://www.youtube.com/watch?v=8C8NnE1Dg4A&t=8s) 讲解了协程的底层实现原理，本质上就是一个状态机，在堆中存储运行过程所需的上下文，通过 `co_await`、`co_yield` 和 `co_return` 来控制状态的转移。

3. [【【协程革命】C++协程上手体验 干爽无废话 纯粹不卖课 全程字幕】](https://www.bilibili.com/video/BV1RV4y1L7ar/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    协程介绍，入门性质，主要是说未来是协程的。

4. [【【协程革命】理论篇！扫盲，辟谣一条龙！全语言通用，夯实基础，准备起飞！   全程字幕】](https://www.bilibili.com/video/BV1K14y1v7cw/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    协程理论，协程的定义（可以挂起和恢复的函数），协程的挂起和恢复是用户控制的，而普通函数是编译器控制的。还介绍了一些协程调度的内容。

5. [【【协程革命】实现篇！无栈协程 有手就行？！ 全程字幕】](https://www.bilibili.com/video/BV1dv4y127YT/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

    使用 switch 和 goto 语句以及一个 frame struct 模拟一个非对称协程斐波那契数列的实现，与 2 中的内容相似，但是更加简单易懂，适合入门。

    代码见 [projects/learn-cpp-coroutine/sim_coroutine_fib.cpp](../projects/learn-cpp-coroutine/sim_coroutine_fib.cpp)。

    协程的对称概念是关于其调度的一部分，C++所采用的对称协程可由用户自定义当前协程暂停之后恢复执行的协程，而非对称协程则只能恢复到调度器。

6. [cpp reference 协程](https://en.cppreference.com/w/cpp/language/coroutines)

    官方文档，稍显晦涩，可以用作手册查阅。

7. [Coroutine Theory](https://lewissbaker.github.io/2017/09/25/coroutine-theory)

    Lewis Baker 的协程理论博客，主要介绍了 C++ 协程的基本概念和工作原理，包括协程与普通函数的区别、协程的操作（挂起、恢复、销毁）、协程的激活帧等内容，有一个具体的讲解协程帧和协程执行栈帧的例子，详细描述了协程恢复挂起过程中两个帧的具体情况。

    值得注意的一点是，C++协程技术规范中有一些规定，允许在编译器能够证明协程的生命周期确实严格嵌套于调用者生命周期内的情况下，从调用者的栈帧中分配协程帧的内存。

8. [C++ Coroutines: Understanding operator co_await](https://lewissbaker.github.io/2017/11/17/understanding-operator-co-await)

    Lewis Baker 的协程理论博客，介绍了 co_await 的实现原理，包括 co_await 的类型、co_await 的返回值类型、co_await 的执行过程等内容。

    关于 Awaitable，await_transform，Awaiter，operator co_await 等概念在文中有详细讲解，具体来说：

    * Awaitable: 可以被 co_await 的类型，包括 Awaiter 和 重载了 operator co_await 的类型。
    * Awaiter: 实现了 await_ready, await_suspend, await_resume 的对象。
    * await_transform: promise_type 中的一个函数，用于将一个类型转换为 Awaitable。
    * operator co_await: 可以是 Awaitable 中的一个重载，也可以是一个全局重载，用于将 Awaitable 转换为 Awaiter。

    `co_await expr` 的执行过程（将 expr 转换为 Awaiter 的过程）伪代码：

    ```cpp
    template<typename P, typename T>
    decltype(auto) get_awaitable(P& promise, T&& expr)
    {
        if constexpr (has_any_await_transform_member_v<P>)
            return promise.await_transform(static_cast<T&&>(expr));
        else
            return static_cast<T&&>(expr);
    }

    template<typename Awaitable>
    decltype(auto) get_awaiter(Awaitable&& awaitable)
    {
        if constexpr (has_member_operator_co_await_v<Awaitable>)
            return static_cast<Awaitable&&>(awaitable).operator co_await();
        else if constexpr (has_non_member_operator_co_await_v<Awaitable&&>)
            return operator co_await(static_cast<Awaitable&&>(awaitable));
        else
            return static_cast<Awaitable&&>(awaitable);
    }
    ```

    `co_await Awaiter` 的执行过程（Awaiter 的 await_suspend 和 await_resume 的执行过程）伪代码：

    ```cpp

    {
    auto&& value = <expr>;
    auto&& awaitable = get_awaitable(promise, static_cast<decltype(value)>(value));
    auto&& awaiter = get_awaiter(static_cast<decltype(awaitable)>(awaitable));
    if (!awaiter.await_ready())
    {
        using handle_t = std::experimental::coroutine_handle<P>;

        using await_suspend_result_t =
        decltype(awaiter.await_suspend(handle_t::from_promise(p)));

        <suspend-coroutine>

        if constexpr (std::is_void_v<await_suspend_result_t>)
        {
        awaiter.await_suspend(handle_t::from_promise(p));
        <return-to-caller-or-resumer>
        }
        else
        {
        static_assert(
            std::is_same_v<await_suspend_result_t, bool>,
            "await_suspend() must return 'void' or 'bool'.");

        if (awaiter.await_suspend(handle_t::from_promise(p)))
        {
            <return-to-caller-or-resumer>
        }

        <resume-point>
    }

    return awaiter.await_resume();
    }
    ```

    无栈协程相比于有栈协程的一大优势在于，无需同步即可编写异步代码，可以免去 future-promise 以及 mutex 等同步机制带来的开销。

    博客最后给出了一个例子，使用协程实现一个生产者-消费者模型，代码见 [projects/learn-cpp-coroutine/thread_sync_primitive.cpp](../projects/learn-cpp-coroutine/thread_sync_primitive.cpp)，这个例子中实现了一个 `async_manual_reset_event` 的同步原语来实现生产者-消费者模型，这个类可以看作是一个同步机制，用于在生产者和消费者之间传递信号。这个实现还是比较有趣的，值得仔细研究。
