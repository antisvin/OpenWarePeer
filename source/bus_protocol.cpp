#include <variant>
//#include "bus_fifo.hpp"
#include "bus_protocol.hpp"

namespace owpeer {

BusProtocolObject* BusDiscover::decodeFrame(const BusFrame& frame) {
    auto new_discover = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_discover = BusDiscover(frame.getSeq(),
        (frame.frame_buffer[1] << 16) | (frame.frame_buffer[2] << 8) |
            frame.frame_buffer[3]);
    return new_discover;
}

BusProtocolObject* BusReset::decodeFrame(const BusFrame& frame) {
    (void)frame;
    auto new_reset = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_reset = BusReset();
    return new_reset;
}

BusProtocolObject* BusMidi::decodeFrame(const BusFrame& frame) {
    auto new_midi = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_midi = BusMidi(frame.frame_buffer[0], frame.frame_buffer[1],
        frame.frame_buffer[2], frame.frame_buffer[3]);
    return new_midi;
}

BusProtocolObject* BusButton::decodeFrame(const BusFrame& frame) {
    PatchButtonId new_bid = PatchButtonId(frame.frame_buffer[1]);
    uint32_t new_value = (frame.frame_buffer[2] << 8) | frame.frame_buffer[3];
    auto new_button = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_button = BusButton(frame.getSeq(), new_bid, new_value);
    return new_button;
}

BusProtocolObject* BusParameter::decodeFrame(const BusFrame& frame) {
    PatchParameterId new_pid = PatchParameterId(frame.frame_buffer[1]);
    uint32_t new_value = (frame.frame_buffer[2] << 8) | frame.frame_buffer[3];
    auto new_parameter = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_parameter = BusParameter(frame.getSeq(), new_pid, new_value);
    return new_parameter;
}

BusProtocolObject* BusCommand::decodeFrame(const BusFrame& frame) {
    auto new_command = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_command = BusCommand(frame.getSeq(), frame.frame_buffer[1],
        (frame.frame_buffer[2] << 8) | frame.frame_buffer[3]);
    return new_command;
}

BusProtocolObject* BusData::decodeFrame(const BusFrame& frame) {
    uint32_t size = (frame.frame_buffer[1] << 16) |
        (frame.frame_buffer[2] << 8) | frame.frame_buffer[3];
    void* data = nullptr;
    if (size < FRAMES_UNKNOWN) {
        data = bus_heap.alloc(size);
    }
    auto new_data = bus_protocol_fifo.takeObjectTimeoutInfinite();
    *new_data = std::move(BusData(frame.getSeq(), (uint8_t*)data, size));
    return new_data;
}

BusData& BusData::operator<<(const BusFrame& frame) {
    if (frames_remaining--) {
        // This handles frame decoding until last frame is reached
        *position++ = frame.frame_buffer[1];
        *position++ = frame.frame_buffer[2];
        *position++ = frame.frame_buffer[3];
    }
    else {
        // Last frame is terminated with zeros, so we have to make a partial load
        switch (len % 3) {
        case 1:
            *position++ = frame.frame_buffer[1];
            break;
        case 2:
            *position++ = frame.frame_buffer[1];
            *position++ = frame.frame_buffer[2];
        default:
            // This clause is occuring if data size is a multiple of 3
            break;
        }
    }
    return *this;
}

BusData& BusData::operator>>(BusFrame& frame) {
    frame.frame_buffer[0] = OWL_COMMAND_DATA | peer;
    if (frames_remaining--) {
        frame.frame_buffer[1] = *position++;
        frame.frame_buffer[2] = *position++;
        frame.frame_buffer[3] = *position++;
        bytes_remaining -= 3;
    }
    else {
        switch (len) {
        case 1:
            frame.fill(*position++, '\0', '\0');
            break;
        case 2:
            frame.frame_buffer[1] = *position++;
            frame.frame_buffer[2] = *position++;
            frame.frame_buffer[3] = '\0';
            break;
        default:
            // This clause should never be reached - it would mean that
            // isEncoded result hasn't been checked before sending last frame
            break;
        };
        bytes_remaining = 0;
    }
    return *this;
}

}