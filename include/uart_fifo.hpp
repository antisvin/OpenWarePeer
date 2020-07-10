#pragma once
#ifndef __UART_FIFO__
#define __UART_FIFO__

#include "chobjfifos.hpp"
#include "bus.hpp"

namespace owpeer {

using namespace chibios_rt;

static constexpr size_t num_frames = 64;
using FramesFifo = ObjectsFifo<BusFrame, num_frames>;

extern FramesFifo rx_fifo;
extern FramesFifo tx_fifo;


}
#endif