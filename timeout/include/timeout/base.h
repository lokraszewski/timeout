/*
 * @Author: Lukasz
 * @Date:   14-03-2019
 * @Last Modified by:   Lukasz
 * @Last Modified time: 14-03-2019
 */

#pragma once
#include <cstddef>
#include <functional>
#include <stdint.h>
namespace timeout
{
/**
 * @author     lokraszewski
 * @date       14-Mar-2019
 * @brief      Base API
 *
 * @details All units are in timer ticks, up to the user.
 */

template <typename interval_t>
class Base
{
public:
  typedef const std::function<void(void)> handler_t;

  virtual void start(const interval_t& ticks, const handler_t& callback) = 0;
  virtual void stop()                                                    = 0;
  virtual bool operator()()                                              = 0;
  virtual bool running()                                                 = 0;
  virtual bool elapsed()                                                 = 0;
};
} // namespace timeout
