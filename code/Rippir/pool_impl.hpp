#pragma once

#include <any>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>

namespace rippir
{

class PoolImpl
{
 private:
  std::mutex q_lock;
  std::condition_variable var;
  std::deque<std::packaged_task<void()>> pending;
  std::vector<std::future<void>> finished;

  void thread_task() {
    while (true) {
      std::packaged_task<void()> f;
      {
        std::unique_lock<std::mutex> l(q_lock);
        if (pending.empty()) {
          var.wait(l, [&] { return !pending.empty(); });
        }
        f = std::move(pending.front());
        pending.pop_front();
      }
      if (!f.valid()) {
        return;
      }
      f();
    }
  }

 public:
  /// @brief Queue a task
  /// @tparam F a function
  /// @tparam ...Params arguments for a function
  /// @param f a pointer to a function
  /// @param ...args
  /// @return a future to an `any`, this will be the return type of f
  template <typename F, typename... Params>
  std::future<std::any> queue(F&& f, Params&&... args) {
    // create a packaged task
    std::packaged_task<std::any()> p(std::bind(std::forward<F>(f), std::forward<Params>(args)...));

    // get a future from the task
    auto r = p.get_future();

    // lock the mutex
    std::unique_lock<std::mutex> l(q_lock);

    // queue the task
    pending.emplace_back(std::move(p));

    // notify a thread
    var.notify_one();

    // return the future result of the task
    return r;
  }

  /// @brief start running N threads
  /// @param N the number of threads
  /// @param policy the launch policy, defaulted to async
  void go(std::size_t N, std::launch policy) {
    for (std::size_t i = 0; i < N; ++i) {
      finished.push_back(std::async(policy, [this] { thread_task(); }));
    }
  }

  /// @brief stop all tasks
  void abort() {
    cancel_pending();
    close();
  }

  /// @brief cancel the queue
  void cancel_pending() {
    std::unique_lock<std::mutex> l(q_lock);
    pending.clear();
  }

  /// @brief closes the PoolImpl
  void close() {
    {
      std::unique_lock<std::mutex> l(q_lock);
      for ([[maybe_unused]] auto&& unused : finished) {
        pending.push_back({});
      }
    }
    var.notify_all();
    finished.clear();
  }

  ~PoolImpl() { this->close(); }

  PoolImpl() = default;
  PoolImpl(const PoolImpl& other) = delete;
  PoolImpl(PoolImpl&& other) = delete;
  PoolImpl& operator=(const PoolImpl& other) = delete;
  PoolImpl& operator=(PoolImpl&& other) = delete;
};

}  // namespace rippir
