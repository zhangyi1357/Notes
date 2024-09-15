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

9. [Structured Concurrency](https://ericniebler.com/2020/11/08/structured-concurrency/)

    Libunifex 库的作者 Eric Niebler 的博客，介绍了 structured concurrency 的概念，指的是一种组织异步计算的方式，确保子操作在其父操作之前完成，正如函数确保在调用者之前完成一样，通过使用结构化并发，可以以类似同步的方式来编写异步代码，以下是一些关键点：
    1. 它将异步生命周期与普通的 C++词法作用域联系起来，使得异步编程可以使用现代 C++的惯用模式。
    2. 它允许我们安全地将局部变量通过引用传递给立即等待的子任务。
    3. 协程是实现结构化并发的主要方式，但不是唯一方式。本质上，结构化并发是以特定模式组织回调，可以通过 libunifex 等库实现，而不仅仅是通过协程。
    4. 与使用 futures 和回调的非结构化并发相比，结构化并发更容易推理正确性和效率。
    5. 它需要对取消有深层支持，以避免不必要的延迟。
    7. 它使得异步代码更易于维护和理解，因为它保持了代码的结构性。

    结构化并发的一个简单例子：

    ```cpp
    cppcoro::task<> computeResult(State & s);

    cppcoro::task<int> doThing() {
        State s;
        co_await computeResult(s);
        co_return s.result;
    }
    ```

10. [Asynchronous Stacks and Scopes](https://ericniebler.com/2021/08/29/asynchronous-stacks-and-scopes/)

    Eric Niebler 的博客，介绍了异步栈和异步作用域的概念。单线程程序中的调用栈体现了程序如何执行到当前位置，而在多线程程序中，程序调用栈则完全无法体现异步链条如何运行到当前位置。异步栈指的是异步函数的激活链条，异步作用域则指的是单个异步函数的运行时状态，包括所有变量和异步栈的上下文。

    在非结构化并发中，根本不存在异步作用域，我们必须使用诸如 shared_ptr 进行资源管理，而基于协程的结构化编程所带来的巨大优势就是可以让异步作用域与词法作用域对齐，资源管理变成了局部变量，依赖于 C++ 最伟大的 `}` “运算符”，无需使用 shared_ptr 等同步机制。

    协程本质上是回调的语法糖。
    即使没有协程，使用正确的库抽象也可以在 C++ 中实现结构化并发。

    11. [What are senders good for anyway?](https://ericniebler.com/2024/02/04/what-are-senders-good-for-anyway/)

    Eric Niebler 的博客，介绍了 Sender 如何实现以及为什么要使用 Sender。

    1. Senders 的主要优势在于提供了一个统一的异步抽象，可以解决不同库之间异步 API 不兼容的问题。
    2. 使用 Sender 的最终用户不需要直接处理复杂的底层细节，可以在协程中直接使用 co_await 来等待 Sender。

## TO BE LEARNED

[@小彭老师](https://space.bilibili.com/263032155) 的自制协程库教程

* 视频：

    [【【C++20】从 0 开始自制协程库，有手就行（上）】 ](https://www.bilibili.com/video/BV1Yz421Z7rZ/?share_source=copy_web&vd_source=07d6eec55261917555a5d7fb4429cab9)

* 代码：

    [co_async](https://github.com/archibate/co_async)

Lewis Baker 的协程博客：

* [Asymmetric Transfer](https://lewissbaker.github.io/)

[@等疾风](https://space.bilibili.com/35186937) 的 co_context 携程库：

* [co_context](https://github.com/Codesire-Deng/co_context)
