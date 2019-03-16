/*
 * @Author: Lukasz
 * @Date:   14-03-2019
 * @Last Modified by:   Lukasz Okraszewski
 * @Last Modified time: 16-03-2019
 */

#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

namespace timeout
{

namespace standard
{

/**
 * @author     lokraszewski
 * @date       14-Mar-2019
 * @brief      Uses the STL components to construct a timer object. You can use this with RAII and the resources will be automatically
 * cleaned up on destruction
 *
 * @details    In this case the units are in ms but it would be easy to take a interval of any time
 */
template <typename interval_t>
class Timer
{
public:
  using handler_t = typename std::function<void(void)>;

  Timer(const bool repeat = false) : m_repeat(repeat) {}

  Timer(const interval_t &interval, const handler_t &callback = [](void) {}, const bool repeat = false) : m_repeat(repeat)
  {
    start(interval, callback);
  }

  ~Timer() { stop(); }

  void start(const interval_t &interval, const handler_t &callback)
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
        auto result = m_terminate.wait_for(locked, interval);

        if (result == std::cv_status::timeout)
        {
          if (m_repeat == false)
            m_stop = true;
          callback();
        }
      }
    });
  }
  void stop()
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
  bool operator()() { return elapsed(); }

  bool running()
  {
    auto l = std::unique_lock<std::mutex>(m_mutex);
    return !m_stop;
  }

  bool elapsed()
  {
    auto l = std::unique_lock<std::mutex>(m_mutex);
    return m_stop;
  }

private:
  const bool              m_repeat;
  bool                    m_stop;
  handler_t               m_handler;
  std::thread             m_thread;
  std::condition_variable m_terminate;
  std::mutex              m_mutex;
};

} // namespace standard
} // namespace timeout
