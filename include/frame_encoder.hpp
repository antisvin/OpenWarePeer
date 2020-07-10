#pragma once
#ifndef __UART_RX__
#define __UART_RX__

#include "ch.hpp"
#include "hal.h"
#include "bus_frame.hpp"

namespace owpeer {

using namespace chibios_rt;

class RxThread : public BaseStaticThread<128> {
private:
    void main (void) override {
        setName("Uart Rx");

        while (true){
            // read from serial
            // send frame to input buffer
        }
    };

    BusFrame<> rx_frame;
};

}

#endif