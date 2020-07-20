#pragma once
#ifndef __MESSAGE_HANDLER__
#define __MESSAGE_HANDLER__

#include "ch.hpp"
#include "bus.hpp"
#include "bus_protocol.hpp"

namespace owpeer {

using namespace chibios_rt;

class MessageHandlerThread : public BaseStaticThread<128> {
private:
    void main() {
        for(;;){
            auto sender = waitMessage();
            auto msg = sender.getMessage();
            auto base_obj = reinterpret_cast<BusObject*>(msg);
            if (base_obj->isMidi()){
                // Not implemented yet
            }
            else {
                switch (base_obj->getObjectType()) {
                case OWL_COMMAND_DISCOVER:
                    chprintf(chp, "DISCOVER RECEIVED");
                    break;
                default:
                    chprintf(chp, "UNHANDLED OBJECT");
                    break;
                }
            }
            sender.releaseMessage(msg);
        }
    }
};

}

#endif