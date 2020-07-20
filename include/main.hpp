#pragma once
#ifndef __MAIN__
#define __MAIN__

#include "ch.hpp"
#include "hal.h"
#include "chobjfifos.hpp"
#include "bus.hpp"
//#include "bus_fifo.hpp"
#include "chprintf.h"
#include "owpeer.h"


/*

void* operator new(size_t size) {
    return chHeapAlloc(0x0, size);
}
void* operator new[](size_t size) {
    return chHeapAlloc(0x0, size);
}
void operator delete(void* p) {
    chHeapFree(p);
}
void operator delete[](void* p) {
    chHeapFree(p);
}
*/

namespace owpeer {

using namespace chibios_rt;

extern BaseSequentialStream* chp;
extern Heap bus_heap;

}

#endif