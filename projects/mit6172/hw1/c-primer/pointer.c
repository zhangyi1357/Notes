// Copyright (c) 2012 MIT License by 6.172 Staff

#include <stdint.h>
#include <stdio.h>

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
  // *pcc = '7'; // invalid? yes, pcc is a pointer to a char constant, the value
  // it points to is const
  pcc = *pcp; // valid? yes, pcc is a pointer to a char constant, but itself is
              // not const
  pcc = argv[0]; // valid? yes, same as above

  char *const cp = c; // cp is a const pointer to char
  // For each of the following, why is the assignment:
  // cp = *pcp;  // invalid? yes, cp is a const pointer to char, so it cannot be
  // assigned to
  // cp = *argv; // invalid? yes, same as above
  // *cp = '!'; // valid? yes, *cp is a char, so it can be assigned to

  const char *const cpc = c; // cpc is a const pointer to char const
  // For each of the following, why is the assignment:
  // cpc = *pcp; // invalid? yes, cpc is a const pointer to char const, so it
  // cannot be assigned to
  // cpc = argv[0]; // invalid? yes, same as above
  // *cpc = '@';    // invalid? yes, *cpc is a const char, so it cannot be
  // assigned

  return 0;
}
