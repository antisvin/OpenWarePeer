#pragma once
#ifndef __FRAME_DECODER__
#define __FRAME_DECODER__

#include "bus_protocol.hpp"
//#include "bus_fifo.hpp"

namespace owpeer {

using namespace chibios_rt;


class FrameDecoderThread : public BaseStaticThread<128> {
public:
    void setMessageHandler(ThreadReference* tref){
        message_handler = tref;
    }

private:
    ThreadReference* message_handler;

    void main (void) override {
        setName("Frame decoder");
        chprintf(chp, "decoder started\r\n");
        BusFrame* frame;
        while (true){
            // read from Rx FIFO
            if (rx_fifo.receiveObjectTimeoutInfinite(&frame) == MSG_OK){
                chprintf(chp, "got frame\r\n");
                auto proto = frame->getOwlProtocolType();
                chprintf(chp, "PROTO %u\r\n",  (int)proto);
                switch (proto) {
                case OWL_COMMAND_DISCOVER: {
                        chprintf(chp, "Received discover\r\n");
                        auto obj = BusDiscover::decodeFrame(*frame);
                        bus_protocol_fifo.sendObject(obj);
                        //message_handler->sendMessage();
                    }
                    break;
                case OWL_COMMAND_BUTTON: {
                        chprintf(chp, "Received button\r\n");
                    }
                    break;
                case OWL_COMMAND_PARAMETER:
                    chprintf(chp, "Received command\r\n");
                    break;
                case OWL_COMMAND_DATA:
                    chprintf(chp, "Received data\r\n");
                    break;
                case OWL_COMMAND_MESSAGE:
                    chprintf(chp, "Received message\r\n");
                    break;
                case OWL_COMMAND_COMMAND:
                    chprintf(chp, "Received command\r\n");
                    break;
                case OWL_COMMAND_RESET:
                    chprintf(chp, "Received reset\r\n");
                    break;
                default:
                    chprintf(chp, "Unknown protocol ID");
                    // Initiate reset?
                    break;
                }
                rx_fifo.returnObject(frame);
            }
            // send frame to app
        }
    };

    BusFrame rx_frame;
};

}

#endif