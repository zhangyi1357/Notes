#include <coroutine>
#include <iomanip>
#include <iostream>
#include <random>

struct CoRet {
  struct promise_type {
    int out_;
    int res_;

    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::terminate(); }
    CoRet get_return_object() {
      return {std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_always yield_value(int value) {
      out_ = value;
      return {};
    }
    void return_value(int value) { res_ = value; }
  };

  std::coroutine_handle<promise_type> h_;
};

struct Note {
  int guess;
};

struct Input {
  bool await_ready() { return false; }
  void await_suspend(std::coroutine_handle<CoRet::promise_type> h) {}
  [[nodiscard]] int await_resume() const { return in_.guess; }

  Note &in_;
};

CoRet Guess(Note &note) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);
  int secret = dis(gen);

  std::cout << "coroutine: The correct answer is " << secret << std::endl;

  Input input{note};
  while (true) {
    int user_guess = co_await input;

    if (user_guess == secret) {
      co_yield 0;
      break;
    }
    co_yield user_guess > secret ? 1 : -1;
  }

  co_return secret;
}

int main() {
  Note note;
  auto ret = Guess(note);
  std::cout << "main: Game starts! The program will automatically guess a "
               "number between 1 and 100."
            << std::endl;

  int low = 1;
  int high = 100;
  int guess_count = 0;

  while (!ret.h_.done()) {
    if (guess_count >= 10) {
      std::cout << "main: Number of guesses exceeded 10, game over."
                << std::endl;
      break;
    }

    note.guess = (low + high) / 2;
    guess_count++;

    std::cout << "main: Guess " << guess_count << ": " << std::setw(2)
              << std::setfill(' ') << note.guess;

    ret.h_.resume();

    if (ret.h_.promise().out_ == 0) {
      std::cout << " = correct answer!" << std::endl;
      break;
    }
    if (ret.h_.promise().out_ == 1) {
      std::cout << " > correct answer" << std::endl;
      high = note.guess - 1;
    } else {
      std::cout << " < correct answer" << std::endl;
      low = note.guess + 1;
    }
    ret.h_.resume();
  }

  ret.h_.resume();
  std::cout << "Game over. The correct answer is " << ret.h_.promise().res_
            << ", it took " << guess_count << " guesses." << std::endl;
}
