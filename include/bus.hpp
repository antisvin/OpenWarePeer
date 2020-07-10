#pragma once
#ifndef __BUS_HPP__
#define __BUS_HPP__

#include <algorithm>
#include <cstdint>

namespace owpeer {

enum BusStatus {
    BUS_STATUS_IDLE      = 0x00,
    BUS_STATUS_DISCOVER  = 0x01,
    BUS_STATUS_CONNECTED = 0x02,
    BUS_STATUS_ERROR     = 0xff,
};

enum OwlProtocol {
    OWL_COMMAND_BUTTON    = 0x90,
    OWL_COMMAND_DISCOVER  = 0xa0,
    OWL_COMMAND_PARAMETER = 0xb0,
    OWL_COMMAND_COMMAND   = 0xc0,
    OWL_COMMAND_MESSAGE   = 0xd0,
    OWL_COMMAND_DATA      = 0xe0,
    OWL_COMMAND_RESET     = 0xf0,
};

enum UsbMidi {
    USB_COMMAND_MISC                = 0x00,       /* reserved */
    USB_COMMAND_CABLE_EVENT         = 0x01,       /* reserved */
    USB_COMMAND_2BYTE_SYSTEM_COMMON = 0x02,       /* e.g. MTC, SongSelect */
    USB_COMMAND_3BYTE_SYSTEM_COMMON = 0x03,       /* e.g. Song Position Pointer SPP */
    USB_COMMAND_SYSEX               = 0x04,
    USB_COMMAND_SYSEX_EOX1          = 0x05,
    USB_COMMAND_SYSEX_EOX2          = 0x06,
    USB_COMMAND_SYSEX_EOX3          = 0x07,
    USB_COMMAND_NOTE_OFF            = 0x08,
    USB_COMMAND_NOTE_ON             = 0x09,
    USB_COMMAND_POLY_KEY_PRESSURE   = 0x0A,
    USB_COMMAND_CONTROL_CHANGE      = 0x0B,
    USB_COMMAND_PROGRAM_CHANGE      = 0x0C,
    USB_COMMAND_CHANNEL_PRESSURE    = 0x0D,
    USB_COMMAND_PITCH_BEND_CHANGE   = 0x0E,
    USB_COMMAND_SINGLE_BYTE         = 0x0F,
};

constexpr size_t frame_size = 4;

class BusFrame {
public:
    BusFrame();
    char frame_buffer[frame_size];

    uint8_t getPeerId(){
        return frame_buffer[0] & 0xf0;
    }

    uint8_t getOwlProtocolId(){
        return frame_buffer[0] & 0x0f;
    }

    OwlProtocol getOwlProtocolType(){
        return OwlProtocol(getOwlProtocolId());
    }

    bool isMidi(){
        auto cmd_id = getOwlProtocolId();
        return cmd_id <= USB_COMMAND_SINGLE_BYTE && cmd_id > USB_COMMAND_CABLE_EVENT;        
    }

};

class BusObject {
public:
    OwlProtocol protocol;
    uint8_t peers;
    uint8_t msg[frame_size - 1];

    BusObject(OwlProtocol protocol, uint8_t peers, const uint8_t* srcmsg)
        : protocol(protocol)
        , peers(peers) {
        std::copy(srcmsg, &srcmsg[frame_size - 1], msg);
    }

};

}

#endif