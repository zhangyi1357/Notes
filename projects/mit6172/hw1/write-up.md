# HW1 Write-up

没有 Q1。

Q2: 回答在 pointer.c 中的注释中提出的问题。例如，为什么有些语句有效，而有些无效？

A2:

```c
int main(int argc, char *argv[]) { // What is the type of argv? char **
  int i = 5;
  // The & operator here gets the address of i and stores it into pi
  int *pi = &i;
  // The * operator here dereferences pi and stores the value -- 5 --
  // into j.
  int j = *pi;

  char c[] = "6.172";
  char *pc = c; // Valid assignment: c acts like a pointer to c[0] here.
  char d = *pc;
  printf("char d = %c\n", d); // What does this print? char d = 6

  // compound types are read right to left in C.
  // pcp is a pointer to a pointer to a char, meaning that
  // pcp stores the address of a char pointer.
  char **pcp;
  pcp = argv; // Why is this assignment valid? argv is a pointer to a pointer to
              // a char

  const char *pcc = c;  // pcc is a pointer to char constant
  char const *pcc2 = c; // What is the type of pcc2? char const *

  // For each of the following, why is the assignment:
  *pcc = '7'; // invalid? yes, pcc is a pointer to a char constant, the value it
              // points to is const
  pcc = *pcp; // valid? yes, pcc is a pointer to a char constant, but itself is
              // not const
  pcc = argv[0]; // valid? yes, same as above

  char *const cp = c; // cp is a const pointer to char
  // For each of the following, why is the assignment:
  cp = *pcp;  // invalid? yes, cp is a const pointer to char, so it cannot be
              // assigned to
  cp = *argv; // invalid? yes, same as above
  *cp = '!';  // valid? yes, *cp is a char, so it can be assigned to

  const char *const cpc = c; // cpc is a const pointer to char const
  // For each of the following, why is the assignment:
  cpc = *pcp;    // invalid? yes, cpc is a const pointer to char const, so it
                 // cannot be assigned to
  cpc = argv[0]; // invalid? yes, same as above
  *cpc = '@';    // invalid? yes, *cpc is a const char, so it cannot be assigned

  return 0;
}
```

Q3: 对于 sizes.c 练习中的每个类型，打印该类型指针的大小。请记住，获取数组或结构的地址需要使用&运算符。
提供您的程序输出（应包括实际类型和指针到该类型的大小）。

A3:

```txt
size of int             :  4 bytes, size of int            * :  8 bytes
size of short           :  2 bytes, size of short          * :  8 bytes
size of long            :  8 bytes, size of long           * :  8 bytes
size of char            :  1 bytes, size of char           * :  8 bytes
size of float           :  4 bytes, size of float          * :  8 bytes
size of double          :  8 bytes, size of double         * :  8 bytes
size of unsigned int    :  4 bytes, size of unsigned int   * :  8 bytes
size of long long       :  8 bytes, size of long long      * :  8 bytes
size of uint8_t         :  1 bytes, size of uint8_t        * :  8 bytes
size of uint16_t        :  2 bytes, size of uint16_t       * :  8 bytes
size of uint32_t        :  4 bytes, size of uint32_t       * :  8 bytes
size of uint64_t        :  8 bytes, size of uint64_t       * :  8 bytes
size of uint_fast8_t    :  1 bytes, size of uint_fast8_t   * :  8 bytes
size of uint_fast16_t   :  8 bytes, size of uint_fast16_t  * :  8 bytes
size of uintmax_t       :  8 bytes, size of uintmax_t      * :  8 bytes
size of intmax_t        :  8 bytes, size of intmax_t       * :  8 bytes
size of __int128        : 16 bytes, size of __int128       * :  8 bytes
```

Q4: 文件 swap.c 包含交换两个整数的代码。使用指针重写 swap() 函数，并在 main() 函数中做出相应更改，以便通过调用 swap() 进行值交换。
使用 make swap 编译代码，并运行程序。/swap。在写作中提供编辑后的代码。通过使用 python 脚本 verifier.py 验证 sizes.c 和 swap.c 的两个大小是否正确。

A4:

sizes.c

```c
// Copyright (c) 2012 MIT License by 6.172 Staff

#include <stdint.h>
#include <stdio.h>

#define PRINT_SIZE(type) printf("size of %s : %zu bytes\n", #type, sizeof(type))
#define PRINT_SIZE_PTR(type)                                                   \
  printf("size of %s* : %zu bytes\n", #type, sizeof(type *))

int main() {
  // Please print the sizes of the following types:
  // int, short, long, char, float, double, unsigned int, long long
  // uint8_t, uint16_t, uint32_t, and uint64_t, uint_fast8_t,
  // uint_fast16_t, uintmax_t, intmax_t, __int128, and student

  PRINT_SIZE(int);
  PRINT_SIZE(short);
  PRINT_SIZE(long);
  PRINT_SIZE(char);
  PRINT_SIZE(float);
  PRINT_SIZE(double);
  PRINT_SIZE(unsigned int);
  PRINT_SIZE(long long);
  PRINT_SIZE(uint8_t);
  PRINT_SIZE(uint16_t);
  PRINT_SIZE(uint32_t);
  PRINT_SIZE(uint64_t);
  PRINT_SIZE(uint_fast8_t);
  PRINT_SIZE(uint_fast16_t);
  PRINT_SIZE(uintmax_t);
  PRINT_SIZE(intmax_t);
  PRINT_SIZE(__int128);

  // Composite types have sizes too.
  typedef struct {
    int id;
    int year;
  } student;

  student you;
  you.id = 12345;
  you.year = 4;

  // Array declaration. Use your macro to print the size of this.
  int x[5];

  // You can just use your macro here instead: PRINT_SIZE("student", you);
  printf("size of %s : %zu bytes \n", "student", sizeof(you));
  PRINT_SIZE(x);

  PRINT_SIZE_PTR(int);
  PRINT_SIZE_PTR(short);
  PRINT_SIZE_PTR(long);
  PRINT_SIZE_PTR(char);
  PRINT_SIZE_PTR(float);
  PRINT_SIZE_PTR(double);
  PRINT_SIZE_PTR(unsigned int);
  PRINT_SIZE_PTR(long long);
  PRINT_SIZE_PTR(uint8_t);
  PRINT_SIZE_PTR(uint16_t);
  PRINT_SIZE_PTR(uint32_t);
  PRINT_SIZE_PTR(uint64_t);
  PRINT_SIZE_PTR(uint_fast8_t);
  PRINT_SIZE_PTR(uint_fast16_t);
  PRINT_SIZE_PTR(uintmax_t);
  PRINT_SIZE_PTR(intmax_t);
  PRINT_SIZE_PTR(__int128);
  PRINT_SIZE_PTR(student);

  return 0;
}

}
```

swap.c

```c
// Copyright (c) 2012 MIT License by 6.172 Staff

#include <stdint.h>
#include <stdio.h>

void swap(int *i, int *j) {
  int temp = *i;
  *i = *j;
  *j = temp;
}

int main() {
  int k = 1;
  int m = 2;
  swap(&k, &m);
  // What does this print?
  printf("k = %d, m = %d\n", k, m);

  return 0;
}
```

Q5: 在 matrix-multiply 目录中，输入 make clean; make 时，您会看到什么？

A5:

```txt
rm -f testbed.o matrix_multiply.o matrix_multiply .buildmode \
        testbed.gcda matrix_multiply.gcda \
        testbed.gcno matrix_multiply.gcno \
        testbed.c.gcov matrix_multiply.c.gcov fasttime.h.gcov
clang -O1 -DNDEBUG -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -c testbed.c -o testbed.o
clang -O1 -DNDEBUG -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -c matrix_multiply.c -o matrix_multiply.o
clang -o matrix_multiply testbed.o matrix_multiply.o -lrt -flto -fuse-ld=gold
```

Q6: 从地址检查器中你看到什么输出？将其粘贴到你的写作中。

A6:

```txt
Setup
Running matrix_multiply_run()...
Elapsed execution time: 0.000000 sec

=================================================================
==21290==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 32 byte(s) in 2 object(s) allocated from:
    #0 0x556a9c0469d9  (/home/zy/Notes/projects/mit6172/hw1/matrix-multiply/matrix_multiply+0x11b9d9) (BuildId: 5b44f967e5ff28a8cadad34fb92f8801ac80e15d)
    #1 0x556a9c08ff89  (/home/zy/Notes/projects/mit6172/hw1/matrix-multiply/matrix_multiply+0x164f89) (BuildId: 5b44f967e5ff28a8cadad34fb92f8801ac80e15d)

Indirect leak of 128 byte(s) in 8 object(s) allocated from:
    #0 0x556a9c0469d9  (/home/zy/Notes/projects/mit6172/hw1/matrix-multiply/matrix_multiply+0x11b9d9) (BuildId: 5b44f967e5ff28a8cadad34fb92f8801ac80e15d)
    #1 0x556a9c090017  (/home/zy/Notes/projects/mit6172/hw1/matrix-multiply/matrix_multiply+0x165017) (BuildId: 5b44f967e5ff28a8cadad34fb92f8801ac80e15d)

Indirect leak of 64 byte(s) in 2 object(s) allocated from:
    #0 0x556a9c0469d9  (/home/zy/Notes/projects/mit6172/hw1/matrix-multiply/matrix_multiply+0x11b9d9) (BuildId: 5b44f967e5ff28a8cadad34fb92f8801ac80e15d)
    #1 0x556a9c08ffd2  (/home/zy/Notes/projects/mit6172/hw1/matrix-multiply/matrix_multiply+0x164fd2) (BuildId: 5b44f967e5ff28a8cadad34fb92f8801ac80e15d)

SUMMARY: AddressSanitizer: 224 byte(s) leaked in 12 allocation(s).
```

Q7：在你修复程序后，运行 `./matrix_multiply -p`，粘贴显示矩阵乘法正确工作的程序输出。

A7:

```txt
Setup
Matrix A:
------------
    3      7      8      1
    7      9      8      3
    1      2      6      7
    9      8      1      9
------------
Matrix B:
------------
    1      3      0      1
    5      5      7      8
    0      1      9      8
    9      3      1      7
------------
Running matrix_multiply_run()...
---- RESULTS ----
Result:
------------
   47     55    122    130
   79     83    138    164
   74     40     75    114
  130     95     74    144
------------
---- END RESULTS ----
Elapsed execution time: 0.000000 sec
```

Q8: 粘贴 Valgrind 显示程序中没有错误的输出。

A8:

```txt
==30734== Memcheck, a memory error detector
==30734== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==30734== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==30734== Command: ./matrix_multiply -p
==30734==
Setup
Matrix A:
------------
    3      7      8      1
    7      9      8      3
    1      2      6      7
    9      8      1      9
------------
Matrix B:
------------
    1      3      0      1
    5      5      7      8
    0      1      9      8
    9      3      1      7
------------
Running matrix_multiply_run()...
---- RESULTS ----
Result:
------------
   47     55    122    130
   79     83    138    164
   74     40     75    114
  130     95     74    144
------------
---- END RESULTS ----
Elapsed execution time: 0.000695 sec
==30734==
==30734== HEAP SUMMARY:
==30734==     in use at exit: 0 bytes in 0 blocks
==30734==   total heap usage: 39 allocs, 39 frees, 1,680 bytes allocated
==30734==
==30734== All heap blocks were freed -- no leaks are possible
==30734==
==30734== For lists of detected and suppressed errors, rerun with: -s
==30734== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```