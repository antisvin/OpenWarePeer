#pragma once
#ifndef __BUS_POOL__
#define __BUS_POOL__

#include <variant>
#include "bus.hpp"
#include "bus_protocol.hpp"
#include "owpeer.h"

namespace owpeer {
using namespace chibios_rt;

class BusDiscover;
class BusReset;
class BusMidi;
class BusButton;
class BusParameter;
class BusCommand;
class BusData;
class BusMessage;

using BusProtocolObject =
    std::variant<BusDiscover, BusReset, BusMidi, BusButton, BusParameter, BusCommand, BusData, BusMessage>;
using BusProtocolFifo = ObjectsFifo<BusProtocolObject, PROTOCOL_OBJECTS_POOL_NUM>;

extern BusProtocolFifo bus_protocol_fifo;

}

#endif