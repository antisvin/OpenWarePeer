#pragma once
#ifndef __UART_RX__
#define __UART_RX__

#include "bus.hpp"
#include "main.hpp"
#include "owpeer.h"

namespace owpeer {

using namespace chibios_rt;

class UartRxThread : public BaseStaticThread<128> {
private:
    void main (void) override {
        setName("Uart Rx");

        while (true){
            // Allocate new frame in RX pool
            auto rx_frame = reinterpret_cast<BusFrame*>(rx_fifo.takeObjectTimeout(TIME_INFINITE));

            // read from serial
            sdRead(&BUS_SERIAL, (uint8_t*)(rx_frame->frame_buffer), frame_size);
            chprintf(chp, "Received [%u,%u,%u,%u]\r\n",
                rx_frame->frame_buffer[0], rx_frame->frame_buffer[1],
                rx_frame->frame_buffer[2], rx_frame->frame_buffer[3]);

            rx_fifo.sendObject((void*)rx_frame);
        }
    };

    BusFrame rx_frame;
};

}

#endif