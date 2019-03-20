/*
 * @Author: Lukasz
 * @Date:   19-11-2018
 * @Last Modified by:   Lukasz
 * @Last Modified time: 18-03-2019
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

// This timer prints info message to the screen every 5s to stop travis build from timeout.
Timer<std::chrono::milliseconds> g_timer(5000ms, [&]() { l_log->info("Test running."); }, true);

TEST_CASE("Single Call backs")
{

  SECTION("Timer 1000ms expires after 2000ms. ")
  {
    bool triggered = false;

    Timer<std::chrono::milliseconds> timer(std::chrono::milliseconds(1000), [&]() { triggered = true; });

    REQUIRE(triggered == false);
    REQUIRE(timer() == false);
    std::this_thread::sleep_for(2000ms);
    REQUIRE(triggered == true);
    REQUIRE(timer() == true);
  }

  SECTION("Timer 2000ms, auto stopped ")
  {
    bool triggered = false;

    Timer<std::chrono::milliseconds> timer(std::chrono::milliseconds(2000), [&]() { triggered = true; });
    REQUIRE(triggered == false);
    REQUIRE(timer() == false);
    std::this_thread::sleep_for(1000ms);
    REQUIRE(triggered == false);
    REQUIRE(timer() == false);
  }
}

TEST_CASE("Repeat counter call backs")
{

  SECTION("Repeat timeout does not expire. ")
  {
    Timer<std::chrono::milliseconds> timer(10ms, []() {}, true);
    REQUIRE(timer() == false);
    std::this_thread::sleep_for(5000ms);
    REQUIRE(timer() == false);
  }

  SECTION("Timer 100ms triggered at least 10 times in 1s. ")
  {
    size_t trig = 0;

    Timer<std::chrono::milliseconds> timer(100ms, [&]() { ++trig; }, true);
    REQUIRE(trig == 0);
    std::this_thread::sleep_for(1050ms);
    REQUIRE(trig >= 10);
  }

  SECTION("Repeat, second timer.")
  {
    size_t                      trig = 0;
    Timer<std::chrono::seconds> timer(2s, [&]() { ++trig; }, true);
    std::this_thread::sleep_for(5s);
    REQUIRE(trig >= 2);
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
      std::this_thread::sleep_for(1s);
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
  SECTION("1000ms")
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
