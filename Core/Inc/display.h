#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <string.h>
#include "game2048.h"

// --- CÁC HÀM PROTOTYPE CÔNG KHAI (PUBLIC FUNCTIONS) ---

/**
 * @brief  Khởi tạo phần cứng màn hình LCD, thiết lập Layer và màu nền mặc định.
 * @retval None
 */
void Display_Init(void);

/**
 * @brief  Cập nhật và vẽ toàn bộ giao diện game lên màn hình (Bảng số + Điểm số).
 * @param  game: Con trỏ trỏ tới struct quản lý trạng thái trò chơi hiện tại.
 * @retval None
 */
void Display_DrawBoard(Game2048 *game);

/**
 * @brief  Hiển thị dòng thông báo trạng thái ở cạnh dưới màn hình.
 * @param  msg: Chuỗi thông báo cần hiển thị (Ví dụ: "GAME OVER", "YOU WIN!").
 * @retval None
 */
void Display_ShowMessage(const char *msg);

#endif /* DISPLAY_H */
