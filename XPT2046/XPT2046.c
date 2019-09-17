//
//		CS   						PB1
//		PENIRQ					PB7
//		SCK							PB13
//		MOSI						PB15
//		MISO						PB14
//
#include "XPT2046.h"
#define XPT2046_HOR_RES     320
#define XPT2046_VER_RES     240
#define XPT2046_X_MIN       300
#define XPT2046_Y_MIN       200
#define XPT2046_X_MAX       3800
#define XPT2046_Y_MAX       3800
#define XPT2046_AVG         4
#define XPT2046_INV         0

#define XPT2046_CS_GPIO_Port GPIOB
#define XPT2046_CS_Pin LL_GPIO_PIN_1
#define XPT2046_IRQ_GPIO_Port GPIOB
#define XPT2046_IRQ_Pin LL_GPIO_PIN_7


#include <main.h>

//#include "cmsis_os.h"                   // CMSIS RTOS header file



int16_t avg_buf_x[XPT2046_AVG];
int16_t avg_buf_y[XPT2046_AVG];
uint8_t avg_last;

#define Y 0x90
#define X 0xD0

#define CS_TOUCH_SELECT()  LL_GPIO_ResetOutputPin(GPIOB, XPT2046_CS_Pin)
#define CS_TOUCH_DESELECT()  LL_GPIO_SetOutputPin(GPIOB, XPT2046_CS_Pin)

void SPI2_init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  LL_GPIO_ResetOutputPin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin);

  /**/
  GPIO_InitStruct.Pin = XPT2046_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(XPT2046_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = XPT2046_IRQ_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(XPT2046_IRQ_GPIO_Port, &GPIO_InitStruct);

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**SPI2 GPIO Configuration  
  PB13   ------> SPI2_SCK
  PB14   ------> SPI2_MISO
  PB15   ------> SPI2_MOSI 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI2, &SPI_InitStruct);
  /* USER CODE BEGIN SPI2_Init 2 */
LL_SPI_Enable(SPI2);
  /* USER CODE END SPI2_Init 2 */

}
uint32_t XPT2046_irq_read(void)
{
    return  LL_GPIO_IsInputPinSet(GPIOB, XPT2046_IRQ_Pin);
}


static uint8_t SendReceive_Data8(SPI_TypeDef *SPIx, uint8_t data)
{
    while(!LL_SPI_IsActiveFlag_TXE(SPIx)) {}
    LL_SPI_TransmitData8 (SPIx, data);
    while(!LL_SPI_IsActiveFlag_RXNE(SPIx)) {}
    // (void) SPIx->DR; //fake Rx read;
    while (LL_SPI_IsActiveFlag_BSY(SPIx));
    return LL_SPI_ReceiveData8(SPIx);
}


void XPT2046_init(void)

{
   SPI2_init();
    CS_TOUCH_SELECT();
    SendReceive_Data8(SPI2, 0X80);
    SendReceive_Data8(SPI2, 0X00);
    SendReceive_Data8(SPI2, 0X00);
    LL_mDelay(1);
    CS_TOUCH_DESELECT();
}
uint16_t XPT2046_GetTouch(uint8_t address)
{
    uint16_t data = 0;
    CS_TOUCH_SELECT();
    SendReceive_Data8(SPI2, address);
    data = SendReceive_Data8(SPI2, 0X00);
    data <<= 8;

    data |= SendReceive_Data8(SPI2, 0X00);
    data >>= 3;

    CS_TOUCH_DESELECT();
    return data;
}

void XPT2046_GetTouch_XY(volatile uint16_t* x_kor, volatile uint16_t* y_kor, uint8_t count_read)
{
    uint8_t i = 0;
    uint16_t tmpx, tmpy, touch_x, touch_y = 0;

    touch_x = XPT2046_GetTouch(X);
    //delay_us(100);
    touch_y = XPT2046_GetTouch(Y);
    for (i = 0; i < count_read; i++) {
        tmpx = XPT2046_GetTouch(X);
        //delay_us(100);
        tmpy = XPT2046_GetTouch(Y);

        if (tmpx == 0)  tmpy = 0;
        else if (tmpy == 0)  tmpx = 0;
    else 
    {
			touch_x = (touch_x + tmpx) / 2;
			touch_y = (touch_y + tmpy) / 2;
    }
      
	}
		*x_kor = touch_x;
		*y_kor = touch_y; 
		
}

 void xpt2046_corr(uint16_t * x, uint16_t * y)
{
#if XPT2046_XY_SWAP 
    int16_t swap_tmp;
    swap_tmp = *x;
    *x = *y;
    *y = swap_tmp;
#endif

    if((*x) > XPT2046_X_MIN)(*x) -= XPT2046_X_MIN;
    else(*x) = 0;

    if((*y) > XPT2046_Y_MIN)(*y) -= XPT2046_Y_MIN;
    else(*y) = 0;

    (*x) = (uint32_t)((uint32_t)(*x) * XPT2046_HOR_RES) /
           (XPT2046_X_MAX - XPT2046_X_MIN);

    (*y) = (uint32_t)((uint32_t)(*y) * XPT2046_VER_RES) /
           (XPT2046_Y_MAX - XPT2046_Y_MIN);

#if XPT2046_X_INV 
    (*x) =  XPT2046_HOR_RES - (*x);
#endif

#if XPT2046_Y_INV 
    (*y) =  XPT2046_VER_RES - (*y);
#endif


}

void xpt2046_avg(uint16_t * x, uint16_t * y)
{
    /*Shift out the oldest data*/
    uint8_t i;
    for(i = XPT2046_AVG - 1; i > 0 ; i--) {
        avg_buf_x[i] = avg_buf_x[i - 1];
        avg_buf_y[i] = avg_buf_y[i - 1];
    }

    /*Insert the new point*/
    avg_buf_x[0] = *x;
    avg_buf_y[0] = *y;
    if(avg_last < XPT2046_AVG) avg_last++;

    /*Sum the x and y coordinates*/
    int32_t x_sum = 0;
    int32_t y_sum = 0;
    for(i = 0; i < avg_last ; i++) {
        x_sum += avg_buf_x[i];
        y_sum += avg_buf_y[i];
    }

    /*Normalize the sums*/
    (*x) = (int32_t)x_sum / avg_last;
    (*y) = (int32_t)y_sum / avg_last;
}



/**
 * Get the current position and state of the touchpad
 * @param data store the read data here
 * @return false: because no ore data to be read
 */
bool XPT2046_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t irq = LV_DRV_INDEV_IRQ_READ;

    if (irq == 0) {
        XPT2046_GetTouch_XY(&x, &y, 1);
        xpt2046_corr(&x, &y);
        // xpt2046_avg(&x, &y);

        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
    }
    else
        data->state = LV_INDEV_STATE_REL;
   // printf("X=%d  Y= %d  m_sec=%d count=%d\n\r", x, y, millis(), count);
    return false;
}
