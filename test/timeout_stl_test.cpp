/*
 * @Author: Lukasz
 * @Date:   19-11-2018
 * @Last Modified by:   Lukasz
 * @Last Modified time: 14-03-2019
 */

#include <array>
#include <iostream>
#include <stdint.h>
#include <string>

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "timeout/base.h"
#include "timeout/impl/timer_stl_ms.h"

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>

static auto l_log = spdlog::stdout_color_mt("catch");

static void sleep_ms(const size_t ms)
{
  const char   wait[] = {'-', '\\', '|', '/', '-'};
  const size_t sz     = 4;

  const size_t ms100 = ms / 100;

  for (size_t i = 0; i < ms100; ++i)
  {
    std::cout << "SLeeping [" << wait[i % sz] << "]" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << '\r' << std::flush;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(ms % 100));
}

TEST_CASE("Single Call backs")
{
  using namespace timeout::standard;

  SECTION("Timer 1000ms expires after 2000ms. ")
  {
    bool triggered = false;

    TimerMs timer(1000, [&]() { triggered = true; });

    REQUIRE(triggered == false);
    sleep_ms(2000);
    REQUIRE(triggered == true);
  }

  SECTION("Timer 2000ms, auto stopped ")
  {
    bool triggered = false;

    TimerMs timer(2000, [&]() { triggered = true; });
    REQUIRE(triggered == false);
    sleep_ms(1000);
    REQUIRE(triggered == false);
  }
}
TEST_CASE("Repeat counter call backs")
{
  using namespace timeout::standard;

  SECTION("Timer 100ms triggered at least 10 times in 1s. ")
  {
    size_t trig = 0;

    TimerMs timer(100, [&]() { ++trig; }, true);
    REQUIRE(trig == 0);
    sleep_ms(1010);
    REQUIRE(trig >= 10);
  }
}

TEST_CASE("Elapsed")
{
  using namespace timeout::standard;
  SECTION("5000ms")
  {
    const size_t required_time   = 1000;
    const size_t required_margin = 10;
    size_t       ms              = 0;
    {
      const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      TimerMs                                     timer(required_time);
      while (timer.running())
      {
        ;
      }
      const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      ms                                              = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }

    const Approx target = Approx(required_time).margin(required_margin);
    l_log->info("Target {}ms +-{}ms, actual: {}", required_time, required_margin, ms);
    REQUIRE(ms == target);
  }
}
