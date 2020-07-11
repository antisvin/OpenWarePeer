#pragma once
#ifndef __BUS_PROTOCOL__
#define __BUS_PROTOCOL__

#include "main.hpp"
#include "bus.hpp"

namespace owpeer {

static constexpr uint8_t NO_UID = 0xff;

/*
 * Bus object base class
 *
 * To decode data from frame, use static decodeFrame method in child classes.
 *
 * Encoding to frame is unsurprisingly done with encodeFrame method. Note that
 * we don't use a virtual function for it.
 *
 * Objects will have object_type attribute set only if is_midi is not set to
 * true, so this flag must be checked before determining object type. This is
 * required because currently we don't have an object ID usable by MIDI events.
 */

class BusObject {
public:
    BusObject(OwlProtocol object_type)
        : object_type(object_type) {};
    BusObject(bool is_midi)
        : is_midi(is_midi) {};
    /*
     * Prohibit copy construction and assignment, but allow move.
     */
    BusObject(const BusObject&) = delete;
    BusObject& operator=(const BusObject&) = delete;
    BusObject(BusObject&&) = default;
    BusObject& operator=(BusObject&&) = default;

    bool isMidi() {
        return is_midi;
    }

    /*
     * Decode frame, loading data into this object
     */
    // static BusObject& decodeFrame(const BusFrame& frame) {};
    /*
     * Encode frame, loading data from this object
     */
    // void encodeFrame(BusFrame& frame) {};
protected:
    OwlProtocol object_type;
    bool is_midi;
};

class BusPeerObject : public BusObject {
public:
    BusPeerObject(OwlProtocol object_type, uint8_t peer)
        : BusObject(object_type)
        , peer(peer) {};
    BusPeerObject& operator++() {
        peer++;
        return *this;
    }
    BusPeerObject& operator--() {
        peer--;
        return *this;
    }

protected:
    uint8_t peer;
};

/*
 * BusDiscover object uses bytes 1-3 from frame data for token
 */
class BusDiscover : public BusPeerObject {
public:
    BusDiscover(uint8_t peer, uint32_t token)
        : BusPeerObject(OWL_COMMAND_DISCOVER, peer)
        , token(token) {
    }

    static BusDiscover decodeFrame(const BusFrame& frame) {
        BusDiscover discover(frame.getSeq(),
            (frame.frame_buffer[1] << 16) | (frame.frame_buffer[2] << 8) |
                frame.frame_buffer[3]);
        return discover;
    }

    void encodeFrame(BusFrame& frame) {
        frame.fill(OWL_COMMAND_DISCOVER | peer, token >> 16, token >> 8, token);
    }

private:
    uint32_t token;
};

/*
 * Bus reset doesn't load anything when decoding frame. When encoding, 4
 * identical reset bytes are written
 */
class BusReset : public BusObject {
    BusReset()
        : BusObject(OWL_COMMAND_RESET) {};

    static BusReset decodeFrame(const BusFrame& frame) {
        (void)frame;
        BusReset reset;
        return reset;
    }

    void encodeFrame(BusFrame& frame) {
        frame.fill(OWL_COMMAND_RESET, OWL_COMMAND_RESET, OWL_COMMAND_RESET, OWL_COMMAND_RESET);
    }
};

/*
 * MIDI event forwarding
 */
class BusMidi : public BusObject {
    BusMidi(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
        : BusObject(true)
        , data1(data1)
        , data2(data2)
        , data3(data3)
        , data4(data4) {
    }
    static BusMidi decodeFrame(const BusFrame& frame) {
        BusMidi midi(frame.frame_buffer[0], frame.frame_buffer[1],
            frame.frame_buffer[2], frame.frame_buffer[3]);
        return midi;
    }
    void encodeFrame(BusFrame& frame) {
        frame.fill(data1, data2, data3, data4);
    }

private:
    uint8_t data1, data2, data3, data4;
};

/*
 * Button state
 */
class BusButton : public BusPeerObject {
    BusButton(uint8_t peer, PatchButtonId bid, int16_t value)
        : BusPeerObject(OWL_COMMAND_BUTTON, peer)
        , bid(bid)
        , value(value) {
    }
    static BusButton decodeFrame(const BusFrame& frame) {
        PatchButtonId new_bid = PatchButtonId(frame.frame_buffer[1]);
        uint32_t new_value = (frame.frame_buffer[2] << 8) | frame.frame_buffer[3];
        BusButton button(frame.getSeq(), new_bid, new_value);
        return button;
    }
    void encodeFrame(BusFrame& frame) {
        frame.fill(OWL_COMMAND_BUTTON | peer, uint8_t(bid), value >> 8, value);
    }

private:
    PatchButtonId bid;
    int16_t value;
};

/*
 * Parameter state
 */
class BusParameter : public BusPeerObject {
public:
    BusParameter(uint8_t peer, PatchParameterId pid, int16_t value)
        : BusPeerObject(OWL_COMMAND_PARAMETER, peer)
        , pid(pid)
        , value(value) {
    }
    static BusParameter decodeFrame(const BusFrame& frame) {
        PatchParameterId new_pid = PatchParameterId(frame.frame_buffer[1]);
        uint32_t new_value = (frame.frame_buffer[2] << 8) | frame.frame_buffer[3];
        BusParameter parameter(frame.getSeq(), new_pid, new_value);
        return parameter;
    }
    void encodeFrame(BusFrame& frame) {
        frame.fill(OWL_COMMAND_PARAMETER | peer, uint8_t(pid), value >> 8, value);
    }

private:
    PatchParameterId pid;
    int16_t value;
};

class BusCommand : public BusPeerObject {
public:
    BusCommand(uint8_t peer, uint8_t cmd, int16_t data)
        : BusPeerObject(OWL_COMMAND_COMMAND, peer)
        , cmd(cmd)
        , data(data) {
    }
    static BusCommand decodeFrame(const BusFrame& frame) {
        BusCommand command(frame.getSeq(), frame.frame_buffer[1],
            (frame.frame_buffer[2] << 8) | frame.frame_buffer[3]);
        return command;
    }
    void encodeFrame(BusFrame& frame) {
        frame.fill(OWL_COMMAND_COMMAND | peer, cmd, data >> 8, data);
    }

private:
    uint8_t cmd;
    int16_t data;
};

class BusData : public BusPeerObject {
public:
    BusData(uint8_t peer, const uint8_t* data, uint32_t len)
        : BusPeerObject(OWL_COMMAND_DATA, peer)
        , data(data)
        , len(len)
        , bytes_remaining(len) {
        frames_remaining = len / 3;
        position = const_cast<uint8_t*>(data);
    }
    bool isAllocated() const {
        return data != nullptr;
    }
    bool isDecoded() const {
        return frames_remaining == FRAMES_UNKNOWN;
    }
    static BusData decodeFrame(const BusFrame& frame) {
        uint32_t size = (frame.frame_buffer[1] << 16) |
            (frame.frame_buffer[2] << 8) | frame.frame_buffer[3];
        void* data = nullptr;
        if (size < FRAMES_UNKNOWN) {
            data = bus_heap.alloc(size);
        }
        BusData bus_data(frame.getSeq(), (uint8_t*)data, size);
        return bus_data;
    }
    BusData& operator<<(const BusFrame& frame) {
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
    bool isEncoded() const {
        return bool(bytes_remaining);
    }
    void encodeFrame(BusFrame& frame) const {
        frame.fill(OWL_COMMAND_DATA | peer, len >> 16, len >> 8, len);
    }
    BusData& operator>>(BusFrame& frame) {
        frame.frame_buffer[0] = OWL_COMMAND_DATA | peer;
        if (frames_remaining--) {
            frame.fill(*position++, *position++, *position++);
            bytes_remaining -= 3;
        }
        else {
            switch (len) {
            case 1:
                frame.fill(*position++, '\0', '\0');
                break;
            case 2:
                frame.fill(*position++, *position++, '\0');
                break;
            default:
                // This clause should never be reached - it would mean that isEncoded
                // result hasn't been checked before sending last frame
                break;
            };
            bytes_remaining = 0;
        }
        return *this;
    }

private:
    const uint8_t* data;
    uint32_t len, bytes_remaining;
    uint32_t frames_remaining;
    uint8_t* position;
    static constexpr uint32_t FRAMES_UNKNOWN = 0xffffffff;
};

static constexpr uint16_t max_msg_len = 256;

class BusMessage : public BusPeerObject {
public:
    BusMessage(uint8_t peer, const char* msg)
        : BusPeerObject(OWL_COMMAND_MESSAGE, peer)
        , msg(msg) {
            position = const_cast<char*>(msg);
            len = strnlen(msg, max_msg_len);
            bytes_remaining = len;
            if (len % 3 == 0) // We want to force sending 0-filled frame
                bytes_remaining += 3;
        };

    static BusData decodeFrame(const BusFrame& frame) {

    }
    void encodeFrame(BusFrame& frame) const {
        if (frames_remaining--){
            frame.fill(*position++, *position++, *position++);
            bytes_remaining -= 3;
        }
        else {
            switch (bytes_remaining) {
            case 1:
                frame.fill(*position++, '\0', '\0');
                break;
            case 2:
                frame.fill(*position++, *position++, '\0');
                break;
            case 3:
                frame.fill('\0', '\0', '\0');
                break;
            default:
                break;
            }
            bytes_remaining = 0;
        }
        
    }
private:
    const char* msg;
    char* position;
    uint16_t len, frames_remaining, bytes_remaining;
};

};

#endif