#include "display.h"
#include "stm32f429i_discovery_lcd.h"
#include <stdio.h>
#include <string.h>
#define TILE_SIZE     52
#define TILE_GAP      4
#define GRID_OFFSET_X 14
#define GRID_OFFSET_Y 40
#define BG_COLOR      0xFFBBADA0
static uint32_t get_tile_color(uint16_t value)
{
    switch (value) {
        case 0:    return 0xFFCDC1B4;
        case 2:    return 0xFFEEE4DA;
        case 4:    return 0xFFEDE0C8;
        case 8:    return 0xFFF2B179;
        case 16:   return 0xFFF59563;
        case 32:   return 0xFFF67C5F;
        case 64:   return 0xFFF65E3B;
        case 128:  return 0xFFEDCF72;
        case 256:  return 0xFFEDCC61;
        case 512:  return 0xFFEDC850;
        case 1024: return 0xFFEDC53F;
        case 2048: return 0xFFEDC22E;
        default:   return 0xFF3C3A32;
    }
}
void Display_Init(void)
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);
    BSP_LCD_SetLayerVisible(1, ENABLE); // <-- Đã thêm dòng này để bật hiển thị
    BSP_LCD_SelectLayer(1);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_Clear(BG_COLOR);
}
void Display_DrawBoard(Game2048 *game)
{
    char buf[16];
    // Vẽ điểm số
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(BG_COLOR);
    sprintf(buf, "SCORE: %lu", game->score);
    BSP_LCD_DisplayStringAt(10, 10, (uint8_t *)buf, LEFT_MODE);
    // Vẽ từng ô
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            uint16_t val = game->board[r][c];
            uint16_t x = GRID_OFFSET_X + c * (TILE_SIZE + TILE_GAP);
            uint16_t y = GRID_OFFSET_Y + r * (TILE_SIZE + TILE_GAP);
            // Vẽ nền ô
            BSP_LCD_SetTextColor(get_tile_color(val));
            BSP_LCD_FillRect(x, y, TILE_SIZE, TILE_SIZE);
            // Vẽ số nếu ô không trống
            if (val != 0) {
                sprintf(buf, "%d", val);
                BSP_LCD_SetTextColor((val <= 4) ? 0xFF776E65 : LCD_COLOR_WHITE);
                BSP_LCD_SetBackColor(get_tile_color(val));
                // Căn giữa chữ trong ô
                uint16_t text_x = x + TILE_SIZE / 2 - (strlen(buf) * 8);
                uint16_t text_y = y + TILE_SIZE / 2 - 8;
                BSP_LCD_DisplayStringAt(text_x, text_y, (uint8_t *)buf, LEFT_MODE);
            }
        }
    }
}
void Display_ShowMessage(const char *msg)
{
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(0, 300, (uint8_t *)msg, CENTER_MODE);
}
