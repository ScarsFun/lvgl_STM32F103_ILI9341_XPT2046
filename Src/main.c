/*----------------------------------------------------------------------------
 *       Simple STM32F103 Demo, using littleVgl:                             *
 *                                         *
 *       LittleVgl V6.0                            *
 *       Custom STM32F103RC breakout Board (72Mhz, 64K ram, 256K Flash) .    * 
 *       LL (STM32 Low Level Drivers).              *
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

#include "main.h"
#include "../lvgl/lvgl.h"
#include "../ili9341/core.h"
#include "../ili9341/lv_driver.h"
//#include "misc/delay.h"
#include "../demo/demo.h"
#include "../XPT2046/XPT2046.h"


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();
  SystemClock_Config();

  lv_init(); // LittleVgl (6.0)
  ILI9341_init();
	

    static lv_disp_buf_t disp_buf;
    static lv_color_t buf_1[LV_HOR_RES_MAX * 15];
    lv_disp_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * 15);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = ILI9341_flush;
    lv_disp_drv_register(&disp_drv);

    xpt2046_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = xpt2046_read;
    lv_indev_drv_register(&indev_drv);

    demo_create();
	

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
    for (;;) {
        LL_mDelay(1);
        lv_task_handler();
        lv_tick_inc(1);
        //printf("lv_ticks = %d\n\r",millis());
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

   if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
    Error_Handler();  
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(72000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(72000000);
}


/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
