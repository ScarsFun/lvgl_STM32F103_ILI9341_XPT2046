#include "core.h"
#include "lv_driver.h"
//#include <stdio.h>

// void ILI9341_flush(struct _disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
void ILI9341_flush(lv_disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
{
	 TFT_CS_RESET;
	ILI9341_setAddressWindow((uint16_t)area->x1, (uint16_t)area->y1, (uint16_t)area->x2, (uint16_t)area->y2);
	uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
	TFT_DC_D;
	Send_DMA_Data16((void*)color_map, size);
    TFT_CS_SET;
    lv_disp_flush_ready(drv);
	TFT_CS_SET;
	//printf("flush \n\r");
}

