#ifndef TEST1_ILI9341_CORE_H
#define TEST1_ILI9341_CORE_H

#include <stm32f10x.h>
#include <stm32f10x_spi.h>
#include "commands.h"
#include "dma.h"

#include "../lvgl/lvgl.h"

#define ILI9341_setAddressWindowToWrite(x1,y1,x2,y2) \
    ILI9341_setAddressWindow(x1, y1, x2, y2); \
    dmaSendCmd(ILI9341_GRAM)

#define ILI9341_setAddressWindowToRead(x1,y1,x2,y2) \
    ILI9341_setAddressWindow(x1, y1, x2, y2); \
    dmaSendCmd(ILI9341_RAMRD)

u16 ILI9341_getWidth(void);
u16 ILI9341_getHeight(void);

void ILI9341_init(void);

void ILI9341_setSpi8(void);
void ILI9341_setSpi16(void);

void ILI9341_setOrientation(u8 o);
void ILI9341_setAddressWindow(u16 x1, u16 y1, u16 x2, u16 y2);

/*---------------------------------------
*      littleVGL function
*----------------------------------------*/


void ILI9341_fillRect(u16 x1, u16 y1, u16 w, u16 h, u16 color);

#endif //TEST1_ILI9341_CORE_H
