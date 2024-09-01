// Example code for blog post 'Understanding Awaitables'
//
// Copyright (c) Lewis Baker

#include <atomic>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <thread>

class async_manual_reset_event {
public:
  explicit async_manual_reset_event(bool initiallySet = false) noexcept;

  // No copying/moving
  async_manual_reset_event(const async_manual_reset_event &) = delete;
  async_manual_reset_event(async_manual_reset_event &&) = delete;
  async_manual_reset_event &
  operator=(const async_manual_reset_event &) = delete;
  async_manual_reset_event &operator=(async_manual_reset_event &&) = delete;

  bool is_set() const noexcept;

  struct awaiter;
  awaiter operator co_await() const noexcept;

  void set() noexcept;
  void reset() noexcept;

private:
  friend struct awaiter;

  // - 'this' => set state
  // - otherwise => not set, head of linked list of awaiter*.
  mutable std::atomic<void *> m_state;
};

struct async_manual_reset_event::awaiter {
  explicit awaiter(const async_manual_reset_event &event) noexcept
      : m_event(event) {}

  [[nodiscard]] bool await_ready() const noexcept;
  [[nodiscard]] bool
  await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept;
  void await_resume() noexcept {}

private:
  friend struct async_manual_reset_event;

  const async_manual_reset_event &m_event;
  std::coroutine_handle<> m_awaitingCoroutine;
  awaiter *m_next;
};

bool async_manual_reset_event::awaiter::await_ready() const noexcept {
  return m_event.is_set();
}

bool async_manual_reset_event::awaiter::await_suspend(
    std::coroutine_handle<> awaitingCoroutine) noexcept {
  // Special m_state value that indicates the event is in the 'set' state.
  const void *const setState = &m_event;

  // Stash the handle of the awaiting coroutine.
  m_awaitingCoroutine = awaitingCoroutine;

  // Try to atomically push this awaiter onto the front of the list.
  void *oldValue = m_event.m_state.load(std::memory_order_acquire);
  do {
    // Resume immediately if already in 'set' state.
    if (oldValue == setState)
      return false;

    // Update linked list to point at current head.
    m_next = static_cast<awaiter *>(oldValue);

    // Finally, try to swap the old list head, inserting this awaiter
    // as the new list head.
  } while (!m_event.m_state.compare_exchange_weak(
      oldValue, this, std::memory_order_release, std::memory_order_acquire));

  // Successfully enqueued. Remain suspended.
  return true;
}

async_manual_reset_event::async_manual_reset_event(bool initiallySet) noexcept
    : m_state(initiallySet ? this : nullptr) {}

bool async_manual_reset_event::is_set() const noexcept {
  return m_state.load(std::memory_order_acquire) == this;
}

void async_manual_reset_event::reset() noexcept {
  void *oldValue = this;
  m_state.compare_exchange_strong(oldValue, nullptr, std::memory_order_acquire);
}

void async_manual_reset_event::set() noexcept {
  // Needs to be 'release' so that subsequent 'co_await' has
  // visibility of our prior writes.
  // Needs to be 'acquire' so that we have visibility of prior
  // writes by awaiting coroutines.
  void *oldValue = m_state.exchange(this, std::memory_order_acq_rel);
  if (oldValue != this) {
    // Wasn't already in 'set' state.
    // Treat old value as head of a linked-list of waiters
    // which we have now acquired and need to resume.
    auto *waiters = static_cast<awaiter *>(oldValue);
    while (waiters != nullptr) {
      // Read m_next before resuming the coroutine as resuming
      // the coroutine will likely destroy the awaiter object.
      auto *next = waiters->m_next;
      waiters->m_awaitingCoroutine.resume();
      waiters = next;
    }
  }
}

async_manual_reset_event::awaiter
    async_manual_reset_event::operator co_await() const noexcept {
  return awaiter{*this};
}

// A simple task-class for void-returning coroutines.
struct task {
  struct promise_type {
    task get_return_object() { return {}; }
    std::suspend_never initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};

// Example for using async_manual_reset_event to synchronize producer and
// consumer
int value;
async_manual_reset_event event;

// A single call to produce a value
void producer() {
  std::cout << "producer: " << std::chrono::system_clock::now() << std::endl;
  value = 100;

  // Publish the value by setting the event.
  event.set();
  std::cout << "producer: " << std::chrono::system_clock::now() << std::endl;
}

// Supports multiple concurrent consumers
task consumer() {
  // Wait until the event is signalled by call to event.set()
  // in the producer() function.

  std::cout << "consumer: " << std::chrono::system_clock::now() << std::endl;
  co_await event;
  std::cout << "consumer: " << std::chrono::system_clock::now() << std::endl;

  // Now it's safe to consume 'value'
  // This is guaranteed to 'happen after' assignment to 'value'
  std::cout << "value: " << value << std::endl;
}

int main() {
  std::thread consumerThread(consumer);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  producer();
  consumerThread.join();
  return 0;
}