/*
 * @Author: Lukasz
 * @Date:   14-03-2019
 * @Last Modified by:   Lukasz
 * @Last Modified time: 14-03-2019
 */

#include "timeout/impl/timer_stl_ms.h"
#include <iostream>
namespace timeout
{

namespace standard
{

/*
  Create a thread that sleeps
 */

TimerMs::TimerMs(const bool repeat) : m_repeat(repeat) {}

TimerMs::TimerMs(const size_t ticks, const handler_t &callback, const bool repeat) : m_repeat(repeat) { start(ticks, callback); }

TimerMs::~TimerMs() { stop(); }

void TimerMs::start(const size_t ticks, const handler_t &callback)
{

  stop();

  {
    auto l = std::unique_lock<std::mutex>(m_mutex);
    m_stop = false;
  }

  m_thread = std::thread([=]() {
    auto locked = std::unique_lock<std::mutex>(m_mutex);

    while (!m_stop) // We hold the mutex that protects stop
    {
      auto result = m_terminate.wait_for(locked, std::chrono::milliseconds(ticks));

      if (result == std::cv_status::timeout)
      {
        if (m_repeat == false)
          m_stop = true;
        callback();
      }
    }
  });
}

void TimerMs::stop()
{

  {
    // Set the predicate
    auto l = std::unique_lock<std::mutex>(m_mutex);
    m_stop = true;
  }

  // Tell the thread the predicate has changed
  m_terminate.notify_one();

  if (m_thread.joinable())
  {
    m_thread.join();
  }
}
size_t TimerMs::operator()() { return 0; }

bool TimerMs::running()
{
  auto l = std::unique_lock<std::mutex>(m_mutex);
  return !m_stop;
}
} // namespace standard

} // namespace timeout
