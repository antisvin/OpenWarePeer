#include "owpeer.h"
#include "main.hpp"
#include "uart_rx.hpp"
#include "frame_decoder.hpp"


namespace owpeer {

FramesFifo rx_fifo, tx_fifo;
UartRxThread uart_rx_thread;
FrameDecoderThread frame_decoder_thread;
static uint8_t bus_heap_buffer[BUS_HEAP_SIZE];
Heap bus_heap((void*)bus_heap_buffer, BUS_HEAP_SIZE);

/*
 * Serial driver config
 */

BaseSequentialStream* chp = (BaseSequentialStream*)&USB_SERIAL;

static const SerialConfig serialcfg = {
  115200,
  0,
  0,
  0
};


/*
 * Application entry point.
 */
extern "C" {
int main(void) {

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();
    sdStart(&USB_SERIAL, NULL);
    sdStart(&BUS_SERIAL, &serialcfg);

    chprintf(chp, "Let's make some noise!\r\n");

    uart_rx_thread.start(NORMALPRIO + 1);
    frame_decoder_thread.start(NORMALPRIO + 1);

    while (true) {
        palClearPad(GPIOA, GPIOA_LED_GREEN);
        //sdRead(&SD4, buffer, 4);
        palSetPad(GPIOA, GPIOA_LED_GREEN);
        chThdSleepMilliseconds(1000);

        //    if (!palReadPad(GPIOC, GPIOC_BUTTON)) {
        // chprintf((BaseSequentialStream *)&SD2, "hello\r\n");
        //    test_execute((BaseSequentialStream *)&SD2, &rt_test_suite);
        //      test_execute((BaseSequentialStream *)&SD2, &oslib_test_suite);
    }
};
}

}