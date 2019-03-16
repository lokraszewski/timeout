/*
 * @Author: Lukasz
 * @Date:   14-03-2019
 * @Last Modified by:   Lukasz Okraszewski
 * @Last Modified time: 16-03-2019
 */

#include <array>
#include <iostream>
#include <stdint.h>
#include <string>

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "timeout/timer.h"

static auto l_log = spdlog::stdout_color_mt("example2");

using namespace timeout::standard;
using namespace std::chrono_literals;

Timer<std::chrono::milliseconds> timer1(1000ms, [&]() { l_log->info("Timer original callback"); }, true);

int main(int argc, char const *argv[])
{
  (void)argc;
  (void)argv;
  l_log->info("The timer callback will continue to execute for the life time of the application. Press 'x' to quit. 'a' to replace the "
              "timer with a different callback, 'b' to replace the timer with yet again differt callback.");

  char opt;

  for (;;)
  {
    std::cin >> opt;

    switch (opt)
    {
    case 'x': return 0;
    case 'a':
      l_log->info("Replace callback");
      timer1.start(1000ms, [&]() { l_log->info("Timer replaced callback."); });
      break;
    case 'b':
      l_log->info("Replace and change frequency");
      timer1.start(200ms, [&]() { l_log->info("Timer more frequent callback."); });
      break;
    }
  }

  return 0;
}
