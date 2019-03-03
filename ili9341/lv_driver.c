#include "core.h"
#include <stdio.h>

void ILI9341_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map)
{
	ILI9341_setAddressWindowToWrite((u16)x1, (u16)y1, (u16)x2, (u16)y2);
	uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
	ILI9341_setSpi16();
	dmaSendData16((void*)color_map, size);
    ILI9341_setSpi8();
    lv_flush_ready();
}

