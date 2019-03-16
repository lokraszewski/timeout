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

static auto l_log = spdlog::stdout_color_mt("example1");

using namespace timeout::standard;
using namespace std::chrono_literals;

Timer<std::chrono::milliseconds> timer1(1000ms, [l_log]() { l_log->info("Timer1 callback."); }, true);
Timer<std::chrono::milliseconds> timer2(3000ms, [l_log]() { l_log->info("Timer2 callback."); }, true);

int main(int argc, char const *argv[])
{
  (void)argc;
  (void)argv;
  l_log->info(
      "Timer callbacks will continue for the life time of the application. Etner 'x' to quit, 'a' to kill timer 1, 'b' to kill timer 2.");

  char opt;

  for (;;)
  {
    std::cin >> opt;

    switch (opt)
    {
    case 'x': return 0;
    case 'a':
      timer1.stop();
      l_log->info("Killing timer 1");
      break;
    case 'b':
      timer2.stop();
      l_log->info("Killing timer 2");
      break;
    }
  }

  return 0;
}
