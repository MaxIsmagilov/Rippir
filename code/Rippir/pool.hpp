#pragma once

#include <memory>

#include "pool_impl.hpp"

namespace rippir
{
class Pool
{
 public:
  template <typename F, typename... Params>
  std::future<std::any> queue(F&& f, Params&&... args) {
    return m_pool->queue(f, args...);
  }

  void go(std::size_t N = 1, std::launch policy = std::launch::async);

  void close();

  void cancel_pending();

  void finish();

  void abort();

  Pool();

 private:
  std::unique_ptr<PoolImpl> m_pool{};

  Pool(const Pool& other) = delete;
  Pool(Pool&& other) = delete;
  Pool& operator=(const Pool& other) = delete;
  Pool& operator=(Pool&& other) = delete;
};

}  // namespace rippir
