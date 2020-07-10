#include "owpeer.h"
#include "main.hpp"

namespace owpeer {

FramesFifo rx_fifo, tx_fifo;


/*
 * Serial driver config
 */

BaseSequentialStream* chp = (BaseSequentialStream*)&SD2;


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