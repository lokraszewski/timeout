# timeout
A simple timeout timer implemenation. Two modes of operations are supported:
* Single shot callback
* Repeat callback

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
