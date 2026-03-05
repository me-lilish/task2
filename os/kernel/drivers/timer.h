#pragma once
#include "../core/ktypes.h"


namespace Timer {


    void init(uint32_t frequency);


    uint32_t get_ticks();


    uint32_t get_uptime_ms();


    void sleep(uint32_t ms);

}
