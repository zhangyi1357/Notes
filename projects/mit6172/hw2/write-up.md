# HW2 Write-up

Q1: 比较 DEBUG=1 代码和编译优化后的 DEBUG=0 代码的 Cachegrind 输出。解释当比较此程序不同版本的性能时，使用指令计数作为时间替代的优缺点。

DEBUG=1 时，Cachegrind 输出如下：
```
valgrind --tool=cachegrind --branch-sim=yes ./sort 131072 1
==28731== Cachegrind, a high-precision tracing profiler
==28731== Copyright (C) 2002-2024, and GNU GPL'd, by Nicholas Nethercote et al.
==28731== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==28731== Command: ./sort 131072 1
==28731==

Running test #0...
Generating random array of 131072 elements
Arrays are sorted: yes
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.225606 sec
sort_a repeated : Elapsed execution time: 0.225452 sec
Generating inverted array of 131072 elements
Arrays are sorted: yes
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.430092 sec
sort_a repeated : Elapsed execution time: 0.431770 sec

Running test #1...
 --> test_zero_element at line 245: PASS

Running test #2...
 --> test_one_element at line 266: PASS
Done testing.
==28731==
==28731== I refs:        620,659,195
==28731==
==28731== Branches:       60,139,177  (57,910,219 cond + 2,228,958 ind)
==28731== Mispredicts:     4,886,401  ( 4,886,079 cond +       322 ind)
==28731== Mispred rate:          8.1% (       8.4%     +       0.0%   )
```

DEBUG=0 时，Cachegrind 输出如下：
```
valgrind --tool=cachegrind --branch-sim=yes ./sort 131072 1
==29527== Cachegrind, a high-precision tracing profiler
==29527== Copyright (C) 2002-2024, and GNU GPL'd, by Nicholas Nethercote et al.
==29527== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==29527== Command: ./sort 131072 1
==29527==

Running test #0...
Generating random array of 131072 elements
Arrays are sorted: yes
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.140648 sec
sort_a repeated : Elapsed execution time: 0.136985 sec
Generating inverted array of 131072 elements
Arrays are sorted: yes
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.275580 sec
sort_a repeated : Elapsed execution time: 0.272845 sec

Running test #1...
 --> test_zero_element at line 245: PASS

Running test #2...
 --> test_one_element at line 266: PASS
Done testing.
==29527==
==29527== I refs:        368,282,163
==29527==
==29527== Branches:       39,577,600  (37,348,634 cond + 2,228,966 ind)
==29527== Mispredicts:     1,269,455  ( 1,269,113 cond +       342 ind)
==29527== Mispred rate:          3.2% (       3.4%     +       0.0%   )
```

优化后程序程序运行时间和指令数量均有大约 30~40% 的减少，分支预测的错误率从 8.1% 降低到 3.2%。

使用指令计数作为时间替代的优缺点：

优点：
1. 与硬件无关，可以在不同的机器上进行比较，而不需要考虑机器的性能等因素，稳定性和可重复性强。
2. 不需要编写计时的代码。

缺点：
1. 程序运行时间除了受指令的数量影响，还受其他因素影响，如 cache 命中率、分支预测的准确性等，还有 IO 和多线程等因素。

Q2: 解释你选择了哪些函数进行内联，并报告你在内联和未内联排序函数之间观察到的性能差异。

A2: 选择内联 merge_i 和 copy_i 函数

```
valgrind --tool=cachegrind --branch-sim=yes ./sort 131072 1
==2905== Cachegrind, a high-precision tracing profiler
==2905== Copyright (C) 2002-2024, and GNU GPL'd, by Nicholas Nethercote et al.
==2905== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==2905== Command: ./sort 131072 1
==2905==

Running test #0...
Generating random array of 131072 elements
Arrays are sorted: yes
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.137394 sec
sort_a repeated : Elapsed execution time: 0.133495 sec
sort_i          : Elapsed execution time: 0.135876 sec
Generating inverted array of 131072 elements
Arrays are sorted: yes
 --> test_correctness at line 217: PASS
sort_a          : Elapsed execution time: 0.281199 sec
sort_a repeated : Elapsed execution time: 0.265678 sec
sort_i          : Elapsed execution time: 0.270303 sec

Running test #1...
 --> test_zero_element at line 245: PASS

Running test #2...
 --> test_one_element at line 266: PASS
Done testing.
==2905==
==2905== I refs:        550,717,826
==2905==
==2905== Branches:       58,948,306  (55,670,732 cond + 3,277,574 ind)
==2905== Mispredicts:     1,819,171  ( 1,818,821 cond +       350 ind)
==2905== Mispred rate:          3.1% (       3.3%     +       0.0%   )
```

可以看到，相比于未进行内联的 sort_a 函数，sort_i 函数的运行时间有略微的缩短，但是仍不及 sort_a 的重复执行。