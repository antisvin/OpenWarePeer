#pragma once
#ifndef __UART_RX__
#define __UART_RX__

#include "main.hpp"
#include "bus.hpp"
#include "uart_fifo.hpp"

namespace owpeer {

class UartRxThread : public BaseStaticThread<128> {
private:
    void main (void) override {
        setName("UART Rx");

        while (true){
            // Allocate new frame in RX pool
            auto rx_frame = rx_fifo.takeObjectTimeoutInfinite();

            // read from serial
            sdRead(&BUS_SERIAL, (uint8_t*)(rx_frame->frame_buffer), frame_size);
            chprintf(chp, "Received [%u,%u,%u,%u]\r\n",
                rx_frame->frame_buffer[0], rx_frame->frame_buffer[1],
                rx_frame->frame_buffer[2], rx_frame->frame_buffer[3]);

            rx_fifo.sendObject(rx_frame);
        }
    };

    BusFrame rx_frame;
};

}

#endif