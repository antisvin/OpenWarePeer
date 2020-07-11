#ifndef __OWPEER__
#define __OWPEER__

/*
 * Number of frames to store in input/output buffers. Currently each frame would
 * use 4 bytes
 */
#define FRAME_BUFFER_SIZE 64

/*
 * First serial device is for USB UART for debug printing
 * Senond UART is for communication over digital bus protocol
 */
#define USB_SERIAL SD2
#define BUS_SERIAL SD4

/*
 * Heap size for bus - set to 16k
 */
#define BUS_HEAP_SIZE 0x8000 

#endif