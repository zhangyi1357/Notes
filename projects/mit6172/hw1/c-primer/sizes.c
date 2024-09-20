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
