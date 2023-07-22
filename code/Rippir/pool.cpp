#include "pool.hpp"

namespace rippir
{

void Pool::go(std::size_t N, std::launch policy) { m_pool->go(N, policy); }

void Pool::close() { m_pool->close(); }

void Pool::cancel_pending() { m_pool->cancel_pending(); }

void Pool::abort() { m_pool->abort(); };

Pool::Pool() : m_pool{} {}

}  // namespace rippir
