#include "stm32f10x.h"
#include "lvgl/lvgl.h"
#include "ili9341/core.h"
#include "ili9341/lv_driver.h"
#include "misc/delay.h"
#include "demo/demo.h"
#include "XPT2046/XPT2046.h"
#include <stdio.h>

 

int main()
{
   lv_init(); // LittleVgl (6.0)
 
	 ILI9341_init();
     static lv_disp_buf_t disp_buf;
     static lv_color_t buf_1[LV_HOR_RES_MAX * 10];
     lv_disp_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX*10);

	 lv_disp_drv_t disp_drv;
	 lv_disp_drv_init(&disp_drv);
	 disp_drv.buffer = &disp_buf;  
	 disp_drv.flush_cb = ILI9341_flush;
	 lv_disp_drv_register(&disp_drv);

	 xpt2046_init();
	 lv_indev_drv_t indev_drv;
	 lv_indev_drv_init(&indev_drv); 
	 indev_drv.type =LV_INDEV_TYPE_POINTER;
	 indev_drv.read_cb =xpt2046_read;
	 lv_indev_drv_register(&indev_drv);  
	
	
	 demo_create();
	 //gui_create();

         for (;;) {
             delay_ms(1);
             lv_task_handler();
             lv_tick_inc(1);
             //printf("lv_ticks = %d\n\r",millis());
         }
}

