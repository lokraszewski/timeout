/*
 * @Author: Lukasz
 * @Date:   19-11-2018
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

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_INTERNAL_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS
#include <catch2/catch.hpp>

static auto l_log = spdlog::stdout_color_mt("catch");

using namespace timeout::standard;
using namespace std::chrono_literals;

static void sleep_ms(const size_t ms)
{
  const char   wait[] = {'-', '\\', '|', '/', '-'};
  const size_t sz     = 4;

  const size_t ms100 = ms / 100;

  for (size_t i = 0; i < ms100; ++i)
  {
    std::cout << "sleeping ... [" << wait[i % sz] << "]" << std::flush;
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

    Timer<std::chrono::milliseconds> timer(std::chrono::milliseconds(1000), [&]() { triggered = true; });

    REQUIRE(triggered == false);
    sleep_ms(2000);
    REQUIRE(triggered == true);
  }

  SECTION("Timer 2000ms, auto stopped ")
  {
    bool triggered = false;

    Timer<std::chrono::milliseconds> timer(std::chrono::milliseconds(2000), [&]() { triggered = true; });
    REQUIRE(triggered == false);
    sleep_ms(1000);
    REQUIRE(triggered == false);
  }
}
TEST_CASE("Repeat counter call backs")
{

  SECTION("Timer 100ms triggered at least 10 times in 1s. ")
  {
    size_t trig = 0;

    Timer<std::chrono::milliseconds> timer(100ms, [&]() { ++trig; }, true);
    REQUIRE(trig == 0);
    sleep_ms(1010);
    REQUIRE(trig >= 10);
  }
}

TEST_CASE("Timeout check")
{
  SECTION("Operation times out after 2 seconds")
  {
    size_t                           pointless_work = 10;
    Timer<std::chrono::milliseconds> t(1000ms);
    bool                             timeout_occured = false;

    while (pointless_work--)
    {
      /*Each pointless work item takes 1s. */
      sleep_ms(1000);
      if (t())
      {
        /* We have timeout!*/
        timeout_occured = true;
        break;
      }
    }

    REQUIRE(timeout_occured == true);
  }
}

TEST_CASE("Elapsed")
{
  SECTION("5000ms")
  {
    const size_t required_time   = 1000;
    const size_t required_margin = 20;
    size_t       ms              = 0;
    {
      const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      Timer<std::chrono::milliseconds>            timer(1000ms);

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
