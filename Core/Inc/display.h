#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_
#include "stm32f429i_discovery_lcd.h"
#include "game2048.h"
void Display_Init(void);
void Display_DrawHeader(uint32_t score);
void Display_DrawBoard(Game2048 *game);
void Display_ShowMessage(const char *msg);
void Display_ClearMessage(void);
#endif /* INC_DISPLAY_H_ */
