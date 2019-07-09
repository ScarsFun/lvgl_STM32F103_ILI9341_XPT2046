#include "core.h"
//#include <stdio.h>

// void ILI9341_flush(struct _disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
void ILI9341_flush(lv_disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
{
	ILI9341_setAddressWindowToWrite((u16)area->x1, (u16)area->y1, (u16)area->x2, (u16)area->y2);
	uint32_t size = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
	ILI9341_setSpi16();
	dmaSendData16((void*)color_map, size);
    ILI9341_setSpi8();
    lv_disp_flush_ready(drv);
	//printf("flush \n\r");
}

