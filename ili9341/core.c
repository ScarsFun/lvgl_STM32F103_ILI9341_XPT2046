#include "core.h"
//#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "../misc/delay.h"


static u16 screen_width  = ILI9341_PIXEL_WIDTH,
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

};

//</editor-fold>

//<editor-fold desc="LCD initialization functions">

static void ILI9341_pinsInit() {
    SPI_InitTypeDef  spiStructure;
    GPIO_InitTypeDef gpioStructure;

    RCC_PCLK2Config(RCC_HCLK_Div2);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN, ENABLE);
    RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_CLK | SPI_MASTER_CLK, ENABLE);

    // GPIO speed by default
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // GPIO for CS/DC/LED/RESET
    gpioStructure.GPIO_Pin  = TFT_CS_PIN | TFT_DC_PIN | TFT_RESET_PIN | TFT_LED_PIN;
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpioStructure);

    // GPIO for SPI
    gpioStructure.GPIO_Pin  = SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MOSI;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SPI_MASTER_GPIO, &gpioStructure);

    // GPIO for SPI
    gpioStructure.GPIO_Pin  = SPI_MASTER_PIN_MISO;
    gpioStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(SPI_MASTER_GPIO, &gpioStructure);

    SPI_StructInit(&spiStructure);
    spiStructure.SPI_Mode              = SPI_Mode_Master;
    spiStructure.SPI_NSS               = SPI_NSS_Soft;
    spiStructure.SPI_CPOL              = SPI_CPOL_High;
    spiStructure.SPI_CPHA              = SPI_CPHA_2Edge;
    spiStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;     // SPI SPEED
    SPI_Init(SPI_MASTER, &spiStructure);

    SPI_Cmd(SPI_MASTER, ENABLE);
}

void ILI9341_reset() {
    TFT_RST_RESET;
    //osDelay(10);
		delay_ms(10);
    TFT_RST_SET;
    //osDelay(50);
	  delay_ms(50);
}

void ILI9341_exitStandby() {
    dmaSendCmd(ILI9341_SLEEP_OUT);
    //osDelay(150);
	  delay_ms(150);
    dmaSendCmd(ILI9341_DISPLAY_ON);
}

static void ILI9341_configure() {
    u8 count;
    u8 *address = (u8 *) init_commands;

    TFT_CS_RESET;
    while (1) {
        count = *(address++);
        if (count-- == 0) break;
        dmaSendCmdCont(*(address++));
        dmaSendDataCont8(address, count);
        address += count;
    }
    TFT_CS_SET;

    ILI9341_setOrientation(ORIENTATION_LANDSCAPE);
}

void ILI9341_init(void) {
    ILI9341_pinsInit();
    dmaInit();

    ILI9341_reset();
    ILI9341_exitStandby();
    ILI9341_configure();

    TFT_LED_SET;
}

//</editor-fold>

//<editor-fold desc="LCD common operations">

void ILI9341_setOrientation(u8 o) {
    if (o == ORIENTATION_LANDSCAPE || o == ORIENTATION_LANDSCAPE_MIRROR) {
        screen_height = ILI9341_PIXEL_WIDTH;
        screen_width  = ILI9341_PIXEL_HEIGHT;
    } else {
        screen_height = ILI9341_PIXEL_HEIGHT;
        screen_width  = ILI9341_PIXEL_WIDTH;
    }
    TFT_CS_RESET;
    dmaSendCmdCont(ILI9341_MAC);
    dmaSendDataCont8(&o, 1);
    TFT_CS_SET;
}

inline void ILI9341_setAddressWindow(u16 x1, u16 y1, u16 x2, u16 y2) {
    u16 pointData[2];

    TFT_CS_RESET;
    dmaSendCmdCont(ILI9341_COLUMN_ADDR);
    pointData[0] = x1;
    pointData[1] = x2;
    ILI9341_setSpi16();
    dmaSendDataCont16(pointData, 2);
    ILI9341_setSpi8();

    dmaSendCmdCont(ILI9341_PAGE_ADDR);
    pointData[0] = y1;
    pointData[1] = y2;
    ILI9341_setSpi16();
    dmaSendDataCont16(pointData, 2);
    ILI9341_setSpi8();
    TFT_CS_SET;
}



inline void ILI9341_fillRect(u16 x1, u16 y1, u16 w, u16 h, u16 color) {
    u32 count = w * h;
    ILI9341_setAddressWindowToWrite(x1, y1, (u16) (x1 + w - 1), (u16) (y1 + h - 1));
    ILI9341_setSpi16();
    dmaFill16(color, count);
    ILI9341_setSpi8();
}

inline u16 ILI9341_getWidth(void) {
    return screen_width;
}

inline u16 ILI9341_getHeight(void) {
    return screen_height;
}

//</editor-fold>

//<editor-fold desc="SPI functions">

inline void ILI9341_setSpi8(void) {
    SPI_MASTER->CR1 &= ~SPI_CR1_SPE; // DISABLE SPI
    SPI_MASTER->CR1 &= ~SPI_CR1_DFF; // SPI 8
    SPI_MASTER->CR1 |= SPI_CR1_SPE;  // ENABLE SPI
}

inline void ILI9341_setSpi16(void) {
    SPI_MASTER->CR1 &= ~SPI_CR1_SPE; // DISABLE SPI
    SPI_MASTER->CR1 |= SPI_CR1_DFF;  // SPI 16
    SPI_MASTER->CR1 |= SPI_CR1_SPE;  // ENABLE SPI
}


// </editor-fold>
