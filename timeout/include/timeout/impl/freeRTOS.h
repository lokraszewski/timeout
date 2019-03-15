/*
 * @Author: Lukasz
 * @Date:   14-03-2019
 * @Last Modified by:   Lukasz
 * @Last Modified time: 15-03-2019
 */

#pragma once

#include "timeout/base.h"
#include <functional>

#ifdef INC_FREERTOS_H // FreeRTOS include is neccessary before this section

namespace timeout
{

namespace free_rtos
{

/**
 * @author     lokraszewski
 * @date       15-Mar-2019
 * @brief      Class for timer with FreeRTOS implemenation.
 *
 * @details    The class creates a task which manages it. A thread handle or timer handle?
 */
class Timer : public Base<TickType_t, std::function<void(void *)>>
{

public:
  using handler_t = typename Base<TickType_t>::handler_t;

  Timer(const bool repeat = false) : m_repeat(repeat), m_timer_handle(nullptr) {}

  Timer(const interval_t &interval, const handler_t &callback = timer_callback, const bool repeat = false)
      : m_repeat(repeat), m_timer_handle(nullptr)
  {
    start(interval, callback);
  }

  ~Timer() { stop(); }

  void start(const interval_t &interval, const handler_t &callback) override
  {
    // Stop any ongoing timer.
    stop();

    // stop must have cleared the handle to nullptr, otherwise we are toast!
    assert(m_timer_handle == nullptr);

    // Create a new timer.
    m_timer_handle = xTimerCreate("",
                                  /* The timer period in ticks, must be
                                  greater than 0. */
                                  interval,
                                  /* The timers will auto-reload themselves
                                  when they expire. */
                                  repeat ? pdTRUE : pdFALSE,
                                  /* The ID is used to store a count of the
                                  number of times the timer has expired, which
                                  is initialised to 0. */
                                  (void *)0,
                                  /* Each timer calls the same callback when
                                  it expires. */
                                  callback);

    assert(m_timer_handle != nullptr);
    const auto started = xTimerStart(m_timer_handle, portMAX_DELAY);
    assert(started == pdPASS);
  }

  void stop() override
  {
    if (m_timer_handle != nullptr)
    {
      if (xTimerDelete(m_timer_handle, portMAX_DELAY) == pdTrue)
      {
        m_timer_handle = nullptr;
      }
    }
  }

  bool operator()() override { return elapsed(); }
  bool running() override { return m_timer_handle && xTimerIsTimerActive(m_timer_handle); }
  bool elapsed() override { return !running(); }

private:
  void          timer_callback(void *);
  const bool    m_repeat;
  TimerHandle_t m_timer_handle;
};

} // namespace free_rtos
} // namespace timeout
#endif /*INC_FREERTOS_H*/
