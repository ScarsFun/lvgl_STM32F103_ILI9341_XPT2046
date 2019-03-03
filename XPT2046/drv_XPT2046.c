#include <stm32f10x.h>
#include <stdbool.h>
//#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "../misc/delay.h"



#define CS_BIT      GPIO_Pin_1
#define PENIRQ_BIT  GPIO_Pin_7


#define Y 0x90
#define X 0xD0

#define CS_TOUCH_SELECT()      GPIO_WriteBit(GPIOB, CS_BIT, Bit_RESET)
#define CS_TOUCH_DESELECT()    GPIO_WriteBit(GPIOB, CS_BIT, Bit_SET)


uint8_t XPT2046_irq_read(void)
{
	return GPIO_ReadInputDataBit(GPIOB, PENIRQ_BIT);
}
void XPT2046_spi_cs_set(uint8_t val)
{
	if (val)
		GPIO_WriteBit(GPIOB, CS_BIT, Bit_SET);
	else
		 GPIO_WriteBit(GPIOB, CS_BIT, Bit_RESET);
}
void XPT2046_SPI2_init(void)
{
        
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    // set to defaults
    GPIO_StructInit(&GPIO_InitStructure); 
    SPI_StructInit(&SPI_InitStructure);

    // Enable clocks , configure pins for SPI
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15; //SCK MOSI
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //MISO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI2 Config */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);
    //------------------ init CS and IRQ pins--------------
    GPIO_InitStructure.GPIO_Pin = PENIRQ_BIT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = CS_BIT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    }

   uint8_t SPI_XchgByte(uint8_t byte)
{
    //while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, byte);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI2);
}

uint8_t SPI_ReceiveByte(void)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, 0);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI2);
}


uint16_t XPT2046_GetTouch(uint8_t address)
{
    uint16_t data = 0;
    CS_TOUCH_SELECT();
    SPI_XchgByte(address);
    data = SPI_XchgByte(0X00);
    data <<= 8;

    data |= SPI_XchgByte(0X00);
    data >>= 3;

    CS_TOUCH_DESELECT();
    return data;
}

void XPT2046_GetTouch_XY(uint16_t* x_kor, uint16_t* y_kor, uint8_t count_read)
{
    uint8_t i = 0;
    uint16_t tmpx, tmpy, touch_x, touch_y;
    touch_x = XPT2046_GetTouch(X); //с
    touch_y = XPT2046_GetTouch(Y); //считыв
    for (i = 0; i < count_read; i++) {
        tmpx = XPT2046_GetTouch(X);
        tmpy = XPT2046_GetTouch(Y);

        if (tmpx == 0)
            tmpy = 0;
        else if (tmpy == 0)
            tmpx = 0;
        else {
            touch_x = (touch_x + tmpx) / 2;
            touch_y = (touch_y + tmpy) / 2;
        }
    }
    *x_kor = touch_x;
    *y_kor = touch_y;
}


