#ifndef TEST2_COMMANDS_H
#define TEST2_COMMANDS_H

//<editor-fold desc="Display commands">

/* Level 1 Commands */
#define ILI9341_SWRESET             0x01   /* Software Reset */
#define ILI9341_READ_DISPLAY_ID     0x04   /* Read display identification information */
#define ILI9341_RDDST               0x09   /* Read Display Status */
#define ILI9341_RDDPM               0x0A   /* Read Display Power Mode */
#define ILI9341_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define ILI9341_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define ILI9341_RDDIM               0x0D   /* Read Display Image Format */
#define ILI9341_RDDSM               0x0E   /* Read Display Signal Mode */
#define ILI9341_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define ILI9341_SPLIN               0x10   /* Enter Sleep Mode */
#define ILI9341_SLEEP_OUT           0x11   /* Sleep out register */
#define ILI9341_PTLON               0x12   /* Partial Mode ON */
#define ILI9341_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define ILI9341_DINVOFF             0x20   /* Display Inversion OFF */
#define ILI9341_DINVON              0x21   /* Display Inversion ON */
#define ILI9341_GAMMA               0x26   /* Gamma register */
#define ILI9341_DISPLAY_OFF         0x28   /* Display off register */
#define ILI9341_DISPLAY_ON          0x29   /* Display on register */
#define ILI9341_COLUMN_ADDR         0x2A   /* Colomn address register */
#define ILI9341_PAGE_ADDR           0x2B   /* Page address register */
#define ILI9341_GRAM                0x2C   /* GRAM register */
#define ILI9341_RGBSET              0x2D   /* Color SET */
#define ILI9341_RAMRD               0x2E   /* Memory Read */
#define ILI9341_PLTAR               0x30   /* Partial Area */
#define ILI9341_VSCRDEF             0x33   /* Vertical Scrolling Definition */
#define ILI9341_TEOFF               0x34   /* Tearing Effect Line OFF */
#define ILI9341_TEON                0x35   /* Tearing Effect Line ON */
#define ILI9341_MAC                 0x36   /* Memory Access Control register*/
#define ILI9341_VSCRSADD            0x37   /* Vertical Scrolling Start Address */
#define ILI9341_IDMOFF              0x38   /* Idle Mode OFF */
#define ILI9341_IDMON               0x39   /* Idle Mode ON */
#define ILI9341_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define ILI9341_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */
#define ILI9341_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */
#define ILI9341_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */
#define ILI9341_GET_SCANLINE        0x45   /* Get Scanline */
#define ILI9341_WDB                 0x51   /* Write Brightness Display register */
#define ILI9341_RDDISBV             0x52   /* Read Display Brightness */
#define ILI9341_WCD                 0x53   /* Write Control Display register*/
#define ILI9341_RDCTRLD             0x54   /* Read CTRL Display */
#define ILI9341_WRCABC              0x55   /* Write Content Adaptive Brightness Control */
#define ILI9341_RDCABC              0x56   /* Read Content Adaptive Brightness Control */
#define ILI9341_WRITE_CABC          0x5E   /* Write CABC Minimum Brightness */
#define ILI9341_READ_CABC           0x5F   /* Read CABC Minimum Brightness */
#define ILI9341_READ_ID1            0xDA   /* Read ID1 */
#define ILI9341_READ_ID2            0xDB   /* Read ID2 */
#define ILI9341_READ_ID3            0xDC   /* Read ID3 */

/* Level 2 Commands */
#define ILI9341_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define ILI9341_FRMCTR1             0xB1   /* Frame Rate Control (In Normal Mode) */
#define ILI9341_FRMCTR2             0xB2   /* Frame Rate Control (In Idle Mode) */
#define ILI9341_FRMCTR3             0xB3   /* Frame Rate Control (In Partial Mode) */
#define ILI9341_INVTR               0xB4   /* Display Inversion Control */
#define ILI9341_BPC                 0xB5   /* Blanking Porch Control register */
#define ILI9341_DFC                 0xB6   /* Display Function Control register */
#define ILI9341_ETMOD               0xB7   /* Entry Mode Set */
#define ILI9341_BACKLIGHT1          0xB8   /* Backlight Control 1 */
#define ILI9341_BACKLIGHT2          0xB9   /* Backlight Control 2 */
#define ILI9341_BACKLIGHT3          0xBA   /* Backlight Control 3 */
#define ILI9341_BACKLIGHT4          0xBB   /* Backlight Control 4 */
#define ILI9341_BACKLIGHT5          0xBC   /* Backlight Control 5 */
#define ILI9341_BACKLIGHT7          0xBE   /* Backlight Control 7 */
#define ILI9341_BACKLIGHT8          0xBF   /* Backlight Control 8 */
#define ILI9341_POWER1              0xC0   /* Power Control 1 register */
#define ILI9341_POWER2              0xC1   /* Power Control 2 register */
#define ILI9341_VCOM1               0xC5   /* VCOM Control 1 register */
#define ILI9341_VCOM2               0xC7   /* VCOM Control 2 register */
#define ILI9341_NVMWR               0xD0   /* NV Memory Write */
#define ILI9341_NVMPKEY             0xD1   /* NV Memory Protection Key */
#define ILI9341_RDNVM               0xD2   /* NV Memory Status Read */
#define ILI9341_READ_ID4            0xD3   /* Read ID4 */
#define ILI9341_PGAMMA              0xE0   /* Positive Gamma Correction register */
#define ILI9341_NGAMMA              0xE1   /* Negative Gamma Correction register */
#define ILI9341_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define ILI9341_DGAMCTRL2           0xE3   /* Digital Gamma Control 2 */
#define ILI9341_INTERFACE           0xF6   /* Interface control register */

/* Extend register commands */
#define ILI9341_POWERA              0xCB   /* Power control A register */
#define ILI9341_POWERB              0xCF   /* Power control B register */
#define ILI9341_DTCA                0xE8   /* Driver timing control A */
#define ILI9341_DTCB                0xEA   /* Driver timing control B */
#define ILI9341_POWER_SEQ           0xED   /* Power on sequence register */
#define ILI9341_3GAMMA_EN           0xF2   /* 3 Gamma enable register */
#define ILI9341_PRC                 0xF7   /* Pump ratio control register */

//</editor-fold>

//<editor-fold desc="Orientation">

#define ORIENTATION_PORTRAIT 0x48
#define ORIENTATION_LANDSCAPE 0x28
#define ORIENTATION_PORTRAIT_MIRROR 0x88
#define ORIENTATION_LANDSCAPE_MIRROR 0xE8

//</editor-fold>

//<editor-fold desc="Dimensions">
#define ILI9341_PIXEL_WIDTH       240
#define ILI9341_PIXEL_HEIGHT      320
#define ILI9341_PIXEL_COUNT    ILI9341_PIXEL_WIDTH * ILI9341_PIXEL_HEIGHT
//</editor-fold>

#endif //TEST2_COMMANDS_H
