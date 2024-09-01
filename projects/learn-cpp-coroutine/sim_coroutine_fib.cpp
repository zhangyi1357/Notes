#include <iostream>

struct fib_frame {
  int state = 0;
  int a;
  int b;
  int c;

  int resume() {
    switch (state) {
    case 0:
      state = 1;
      goto s0;

    case 1:
      state = 2;
      goto s1;

    case 2:
      state = 3;
      goto s2;

    case 3:
      goto s3;
    }

  s0:
    a = b = 1;
    return a;
  s1:
    return b;
  s2:
    while (true) {
      c = a + b;
      return c;
    s3:
      a = b;
      b = c;
    }
  }
};

int main() {
  fib_frame frame;
  for (int i = 0; i < 10; i++) {
    std::cout << frame.resume() << std::endl;
  }
}
