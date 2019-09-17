#ifndef TEST1_ILI9341_CORE_H
#define TEST1_ILI9341_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "commands.h"
#include "control.h"


//#include "../lvgl/lvgl.h"


uint16_t ILI9341_getWidth(void);
uint16_t ILI9341_getHeight(void);

void ILI9341_init(void);

void ILI9341_setOrientation(uint8_t o);
void ILI9341_setAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Send_DMA_Data16(uint16_t* buff, uint16_t dataSize);

/*---------------------------------------
*      littleVGL function
*----------------------------------------*/


void ILI9341_fillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color);

#endif //TEST1_ILI9341_CORE_H
