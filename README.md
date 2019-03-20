# timeout
A cross platform header only C++ timeout timer implementation. Two modes of operations are supported:
* Single shot callback
* Repeat callback

# Requirements
* cmake
* conan
    - spdlog (used in examples)
    - catch2 (tests)

# Usage
## Timeouts
The timers are RAII compatible, this allows the following usage:
```C++

#include "timeout/timer.h"

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
            //timeout has occurred, handle it!
        }
    }
}

```
If the function returns early, the timer will automatically be stopped and the call-back function will not be invoked.

## Period Timers
The timer can also be used to perform a periodic action. For example:
```C++
#include "timeout/timer.h"
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
