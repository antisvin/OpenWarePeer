#pragma once
#ifndef __UART_TX__
#define __UART_TX__

#include "ch.hpp"
#include "hal.h"
#include "bus_frame.hpp"

namespace owpeer {

using namespace chibios_rt;

class UartTxThread : public BaseStaticThread<128> {
private:
    void main (void) override {
        setName("Uart Tx");

        while (true){
            // receive frame from output buffer
            // write ot serial
        }
    };

    BusFrame<> tx_frame;
};

}

#endif