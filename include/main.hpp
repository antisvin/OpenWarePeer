#pragma once
#ifndef __MAIN__
#define __MAIN__

#include "ch.hpp"
#include "hal.h"
#include "chobjfifos.hpp"
#include "bus.hpp"
#include "chprintf.h"

namespace owpeer {

using namespace chibios_rt;

static constexpr size_t num_frames = 64;

using FramesFifo = ObjectsFifo<BusFrame, num_frames>;

extern FramesFifo rx_fifo;
extern FramesFifo tx_fifo;

extern BaseSequentialStream* chp;

}

#endif