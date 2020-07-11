#pragma once
#ifndef __MAIN__
#define __MAIN__

#include "ch.hpp"
#include "hal.h"
#include "chobjfifos.hpp"
#include "bus.hpp"
#include "chprintf.h"
#include "owpeer.h"

namespace owpeer {

using namespace chibios_rt;

extern BaseSequentialStream* chp;
extern Heap bus_heap;

}

#endif