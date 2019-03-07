/*----------------------------------------------------------------------------
 *       Simple STM32F103 Demo, using littleVgl:                             *
 *       Custom STM32F103RC breakout Board (72Mhz, 64K ram, 256K Flash) .    * 
 *       SPL (STM32 Standard Peripheral Libraries).
 *       ILI9341 display over SPI with DMA,                                  *
 *       XPT2046 resistive touch panel                                       * 
 *                                                                           * 
 *       ILI9341 SPI1 CONNECTIONS:                                           *
 *       -------------------------------                                     * 
 *       TFT_RESET                   PA2                                     * 
 *       TFT_DC                      PA3                                     * 
 *       TFT_CS                      PA4                                     * 
 *       TFT_SCK                     PA5                                     * 
 *       TFT_MISO                    PA6                                     * 
 *       TFT_MOSI                    PA7                                     * 
 *                                                                           * 
 *       XPT2046 SPI2 CONNECTIONS:                                           *
 *       --------------------------------                                    * 
 *       TOUCH_CLK                   PB13                                    * 
 *       TOUCH_DIN (MOSI)            PB15                                    *
 *       TOUCH_DO (MISO)             PB14                                    *
 *       TOUCH_CS                    PB1                                     *             
 *       TOUCH_IRQ                   PB7                                     * 
 *                                                                           * 
 *                                                                           * 
 *                                                                           * 
 *---------------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "lvgl/lvgl.h"
#include "ili9341/core.h"
#include "ili9341/lv_driver.h"
#include "misc/delay.h"
#include "XPT2046/XPT2046.h"
#include "demo/demo.h"


int main()
{
    lv_init();

    ILI9341_init();
    lv_disp_drv_t disp;
    lv_disp_drv_init(&disp);
    disp.disp_flush = ILI9341_flush;
    lv_disp_drv_register(&disp);

    xpt2046_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = xpt2046_read;
    lv_indev_drv_register(&indev_drv);

    demo_create();

    for (;;) {
        delay_ms(10);
        lv_task_handler();
        lv_tick_inc(10);
    }
}
