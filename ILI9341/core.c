#include "core.h"
//#include "cmsis_os2.h"                   // CMSIS RTOS header file
//#include "../misc/delay.h"

volatile uint32_t flag_DMA_CH3_bsy;

static uint16_t screen_width  = ILI9341_PIXEL_WIDTH,
    screen_height = ILI9341_PIXEL_HEIGHT;

static const uint8_t init_commands[] = {
    4, 0xEF, 0x03, 0x80, 0x02,
    4, 0xCF, 0x00, 0xC1, 0x30,
    5, 0xED, 0x64, 0x03, 0x12, 0x81,
    4, 0xE8, 0x85, 0x00, 0x78,
    6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
    2, 0xF7, 0x20,
    3, 0xEA, 0x00, 0x00,
    2, ILI9341_POWER1, 0x23, // Power control VRH[5:0]
    2, ILI9341_POWER2, 0x10, // Power control SAP[2:0];BT[3:0]
    3, ILI9341_VCOM1, 0x3e, 0x28, // VCM control
    2, ILI9341_VCOM2, 0x86, // VCM control2
    2, ILI9341_MAC, 0x48, // Memory Access Control
    2, 0x37, 0x00, // Vertical scroll zero
    2, ILI9341_PIXEL_FORMAT, 0x55,
    3, 0xB1, 0x00, 0x18,
    4, 0xB6, 0x08, 0x82, 0x27, // Display Function Control
    2, 0xF2, 0x00, // 3Gamma Function Disable
    2, 0x26, 0x01, // Gamma curve selected
    16, 0xE0, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
    16, 0xE1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
    2, 0x11, 0x80, // Exit Sleep
    2, 0x29, 0x80, // Display on
	  0
};

//</editor-fold>

//<editor-fold desc="LCD initialization functions">

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, TFT_RESET_Pin|TFT_DC_Pin|TFT_CS_Pin);

  /**/
  GPIO_InitStruct.Pin = TFT_RESET_Pin|TFT_DC_Pin|TFT_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration  
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
/*
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
*/
  /* SPI1 DMA Init */
  
  /* SPI1_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_HALFWORD);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_HALFWORD);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

static void MX_DMA_Init(void) 
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
    // Assign request line
   // LL_DMA_SetPeriphRequest(DMA1,LL_DMA_CHANNEL_3, LL_DMA_REQUEST_1);

  // Enable DMA interrupts on DMA Tx complete and DMA error 
  //LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);  // DMA Tx Complete IT
  //LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);  // DMA error
    // note: this will set IT flags internally to DMA periph
    // but won't trigger IT on Cortex core as long as NVIC not set
  /* DMA1_Channel3_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

void Send_DMA_Data16(uint16_t* buff, uint16_t dataSize)
{
    LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_16BIT);
     
    LL_SPI_Disable(SPI1);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_ClearFlag_TC3(DMA1);
    LL_DMA_ClearFlag_TE3(DMA1);
    LL_SPI_EnableDMAReq_TX(SPI1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, dataSize);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_3, (uint32_t)buff, LL_SPI_DMA_GetRegAddr(SPI1), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3));
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_SPI_Enable(SPI1);
    while (!flag_DMA_CH3_bsy) {
    }
    flag_DMA_CH3_bsy = 0;
   
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_SPI_Disable(SPI1);
    LL_DMA_ClearFlag_TC3(DMA1);
    LL_DMA_ClearFlag_TE3(DMA1);
    LL_SPI_DisableDMAReq_TX(SPI1);
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_DisableIT_TE(DMA1, LL_DMA_CHANNEL_3);
    LL_SPI_Enable(SPI1);  
    LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
}
void DMA1_Channel3_TransferComplete(void)
{
  LL_DMA_ClearFlag_GI3(DMA1);
  flag_DMA_CH3_bsy = 1;
}

static void Send_Data8(uint8_t data)
{
    while(!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
    LL_SPI_TransmitData8 (SPI1, data);
		//while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
		(void) SPI1->DR; //fake Rx read;
    while (LL_SPI_IsActiveFlag_BSY(SPI1)){}
}
static void Send_Data16(uint16_t data)
{

    while(!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
    LL_SPI_TransmitData16 (SPI1, data);
		//while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) {}
    (void) SPI1->DR; //fake Rx read;
    while (LL_SPI_IsActiveFlag_BSY(SPI1)){}
	 
		}

static void ILI9341_WriteCommand(uint8_t cmd)
{
    TFT_DC_C;
    Send_Data8(cmd);

}
static void ILI9341_WriteData(uint8_t* buff, uint16_t buff_size)
{
   TFT_DC_D;
    for(uint16_t index =0; index<buff_size; index++)
    {
        Send_Data8( buff[index]);
        }

}

static void ILI9341_WriteData16(uint16_t* buff, uint16_t buff_size)
{
    TFT_DC_D;
   LL_SPI_SetDataWidth(SPI1,LL_SPI_DATAWIDTH_16BIT );
    for(uint16_t index =0; index<buff_size; index++)
    {
        Send_Data16( buff[index]);
        }
    LL_SPI_SetDataWidth(SPI1,LL_SPI_DATAWIDTH_8BIT );

}


void ILI9341_reset() {
    TFT_RST_RESET;
    // osDelay(10);
    LL_mDelay(10);
    TFT_RST_SET;
   // osDelay(50);
    LL_mDelay(50);
}
void ILI9341_exitStandby() {
    ILI9341_WriteCommand(ILI9341_SLEEP_OUT);
    //osDelay(150);
	  LL_mDelay(150);
    ILI9341_WriteCommand(ILI9341_DISPLAY_ON);
	  LL_mDelay(150);
}

static void ILI9341_configure() {
    uint8_t count;
    uint8_t *address = (uint8_t *) init_commands;

    TFT_CS_RESET;
    while (1) {
        count = *(address++);
        if (count-- == 0) break;
        ILI9341_WriteCommand(*(address++));
        ILI9341_WriteData(address, count);
        address += count;
			LL_mDelay(10);
    }
    TFT_CS_SET;

    ILI9341_setOrientation(ORIENTATION_LANDSCAPE);
}

void ILI9341_init(void) {
 MX_GPIO_Init();
 MX_DMA_Init();
 MX_SPI1_Init();

 LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
 LL_SPI_Enable(SPI1);

    ILI9341_reset();
    ILI9341_exitStandby();
    ILI9341_configure();

   // TFT_LED_SET;
}

//</editor-fold>

//<editor-fold desc="LCD common operations">

void ILI9341_setOrientation(uint8_t o) {
    if (o == ORIENTATION_LANDSCAPE || o == ORIENTATION_LANDSCAPE_MIRROR) {
        screen_height = ILI9341_PIXEL_WIDTH;
        screen_width  = ILI9341_PIXEL_HEIGHT;
    } else {
        screen_height = ILI9341_PIXEL_HEIGHT;
        screen_width  = ILI9341_PIXEL_WIDTH;
    }
    TFT_CS_RESET;
    ILI9341_WriteCommand(ILI9341_MAC);
    ILI9341_WriteData(&o, 1);
    TFT_CS_SET;
}

inline void ILI9341_setAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint16_t pointData[2];

    
    ILI9341_WriteCommand(ILI9341_COLUMN_ADDR);
    pointData[0] = x1;
    pointData[1] = x2;
    ILI9341_WriteData16(pointData, 2);

    ILI9341_WriteCommand(ILI9341_PAGE_ADDR);
    pointData[0] = y1;
    pointData[1] = y2;
    ILI9341_WriteData16(pointData, 2);

    ILI9341_WriteCommand(ILI9341_GRAM);
    
}


inline void ILI9341_fillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color) {
    TFT_CS_RESET;
    uint16_t tbuf[w];
    ILI9341_setAddressWindow(x1, y1, (uint16_t) (x1 + w - 1), (uint16_t) (y1 + h - 1));
	//ILI9341_setAddressWindow(x1, y1, (uint16_t) w , (uint16_t)  h );
    TFT_DC_D
    for (int x = w ; x >= 0; x--) 
            tbuf[x] = color;
    for (y1 = h; y1 > 0; y1--) 
        Send_DMA_Data16(tbuf,w);
    
    TFT_CS_SET;
}

inline uint16_t ILI9341_getWidth(void) {
    return screen_width;
}

inline uint16_t ILI9341_getHeight(void) {
    return screen_height;
}


