/**
 * @file XPT2046.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "XPT2046.h"
#include "drv_XPT2046.h"
#include "../misc/delay.h"
#include <stdio.h>
#if USE_XPT2046

#include <stddef.h>
#include LV_DRV_INDEV_INCLUDE
#include LV_DRV_DELAY_INCLUDE


/*********************
 *      DEFINES
 *********************/



/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void xpt2046_corr(uint16_t * x, uint16_t * y);
static void xpt2046_avg(uint16_t * x, uint16_t * y);

/**********************
 *  STATIC VARIABLES
 **********************/
int16_t avg_buf_x[XPT2046_AVG];
int16_t avg_buf_y[XPT2046_AVG];
uint8_t avg_last;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the XPT2046
 */
void xpt2046_init(void)
{
    XPT2046_SPI2_init();

    LV_DRV_INDEV_SPI_CS(0);
    SPI_XchgByte(0X80);
    SPI_XchgByte(0X00);
    SPI_XchgByte(0X00);
    delay_ms(25);
    LV_DRV_INDEV_SPI_CS(1);
}

/**
 * Get the current position and state of the touchpad
 * @param data store the read data here
 * @return false: because no more data to be read
 */
bool xpt2046_read(lv_indev_data_t* data)
{
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

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void xpt2046_corr(uint16_t * x, uint16_t * y)
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


static void xpt2046_avg(uint16_t * x, uint16_t * y)
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

#endif
