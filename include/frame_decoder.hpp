#pragma once
#ifndef __DECODER__
#define __DECODER__

#include "ch.hpp"
#include "hal.h"
#include "bus_frame.hpp"
#include "main.hpp"

namespace owpeer {

using namespace chibios_rt;


class FrameDecoderThread : public BaseStaticThread<128> {
private:
    void main (void) override {
        setName("Frame decoder");

        BusFrame tmp_frame;
        while (true){
            // read from Rx FIFO
            if (rx_fifo.receiveObjectTimeout((void**)(&tmp_frame), TIME_INFINITE) == MSG_OK){
                switch (tmp_frame.getCommandType()) {
                case OWL_COMMAND_DISCOVER:
                    chprintf(chp, "Received discover\r\n");
                    break;
                case OWL_COMMAND_BUTTON:
                    break;
                case OWL_COMMAND_PARAMETER:
                    break;
                case OWL_COMMAND_DATA:
                    break;
                case OWL_COMMAND_MESSAGE:
                    break;
                case OWL_COMMAND_COMMAND:
                    break;
                }
            }
            // send frame to app
        }
    };

    BusFrame rx_frame;
};

}

#endif