# timeout
A simple timeout timer implemenation. Two modes of operations are supported:
* Single shot callback
* Repeat callback

# Requirements
* cmake 
* conan 
    - spdlog (used in examples)
    - catch2 (tests)
* C++17 

# Usage
## Timeouts
The timers are RAII compatible, this allows the following usage:
```C++

#include "timeout/impl/timer_stl.h" //select which impl

using namespace timeout::standard;
using namespace std::chrono_literals;

void some_work_function(/*param*/)
{
    Timer<std::chrono::milliseconds> timeout(1000ms); //Timeout after 1s

    for(;;)
    {
        //Do work.
        if(timeout())
        {
            //timeout has occured, handle it!
        }
    }
}

```
If the function returns early, the timer will automatically be stopped and the callback function will not be invoked. 

## Period Timers
The timer can also be used to perform a periodic action. For example:
```C++

#include "timeout/impl/timer_stl.h" //select which impl

using namespace timeout::standard;
using namespace std::chrono_literals;

int main(int argc, char const *argv[])
{
    Timer<std::chrono::milliseconds> timer1(1000ms, [&]() 
    { 
        /* callback work to do every 1000ms*/ 
    }, 
    true /*period reload enable*/
    ); //This can also be global scope.


    for(;;)
    {
        //Main app
    }
}

```
## Other
Please check the `examples` folder for more.

# Build Tests + Example
```
mkdir build && cd build
cmake ..
make
```

# Current support
Currently the library supports STL based timers but hopefully this can be implemented for embedded platforms.

The api is the following format:
```C++

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
```

Example implemenation would follow:
```C++
template <typename interval_t>
class Timer : public Base<interval_t>
{
public:
  using handler_t = typename Base<interval_t>::handler_t;
  /* implement all required calls here */
}
```
Note that we must forward the template parameter to the Base class. 

The current convention is as follows:
* `start` stops the current timer and starts a timer with specified intveral and callback. This is threadsafe.
* `stop` stops the current timer and termiantes any threads. 
* `operator()` returns `elapsed()` as a handy shortcut.
* `running()` returns if the timer is running.
* `elapsed()` returns if the timer is not running. Redudant but looks neater in certain contexts.

The timer stops itself if the destructor is invoked (important)!

# TODOs
* Implement FreeRTOS timers
