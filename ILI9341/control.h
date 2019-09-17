#ifndef TEST2_CONTROL_H
#define TEST2_CONTROL_H

//#include "config.h"
#include "commands.h"


#define TFT_DC_D      LL_GPIO_SetOutputPin(TFT_DC_GPIO_Port, TFT_DC_Pin);
#define TFT_DC_C     LL_GPIO_ResetOutputPin(TFT_DC_GPIO_Port, TFT_DC_Pin);

#define TFT_RST_SET     LL_GPIO_SetOutputPin(TFT_RESET_GPIO_Port, TFT_RESET_Pin);
#define TFT_RST_RESET   LL_GPIO_ResetOutputPin(TFT_RESET_GPIO_Port, TFT_RESET_Pin);

#define TFT_CS_SET       LL_GPIO_SetOutputPin(TFT_CS_GPIO_Port, TFT_CS_Pin);
#define TFT_CS_RESET     LL_GPIO_ResetOutputPin(TFT_CS_GPIO_Port, TFT_CS_Pin);

#define TFT_LED_SET       LL_GPIO_SetOutputPin(TFT_LED_GPIO_Port, TFT_LED_Pin);
#define TFT_LED_RESET     LL_GPIO_ResetOutputPin(TFT_LED_GPIO_Port, TFT_LED_Pin);


#endif //TEST2_CONTROL_H
