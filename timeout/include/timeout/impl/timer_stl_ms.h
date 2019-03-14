/*
 * @Author: Lukasz
 * @Date:   14-03-2019
 * @Last Modified by:   Lukasz
 * @Last Modified time: 14-03-2019
 */

#pragma once
#include "timeout/base.h"
#include <condition_variable>
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
class TimerMs : public Base
{
private:
  const bool              m_repeat;
  bool                    m_stop;
  handler_t               m_handler;
  std::thread             m_thread;
  std::condition_variable m_terminate;
  std::mutex              m_mutex;

public:
  TimerMs(const bool repeat = false);
  TimerMs(const size_t ticks, const handler_t &callback = [](void) {}, const bool repeat = false);
  ~TimerMs();
  void   start(const size_t ticks, const handler_t &callback) override;
  void   stop() override;
  size_t operator()() override;
  bool   running() override;
};

} // namespace standard
} // namespace timeout
