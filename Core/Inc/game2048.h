#ifndef INC_GAME2048_H_
#define INC_GAME2048_H_

#include <stdint.h>
#include <stdbool.h>

#define GRID_SIZE 4

typedef struct {
    uint16_t board[GRID_SIZE][GRID_SIZE]; /* Giá trị ô (0 = trống) */
    uint32_t score;                       /* Điểm hiện tại         */
    bool     won;                         /* Đã đạt ô 2048?        */
    bool     over;                        /* Hết nước đi?          */
} Game2048;

/* Khởi tạo / reset bàn cờ */
void Game_Init(Game2048 *game);

/* Di chuyển — trả về true nếu bàn cờ thay đổi */
bool Game_MoveLeft (Game2048 *game);
bool Game_MoveRight(Game2048 *game);
bool Game_MoveUp   (Game2048 *game);
bool Game_MoveDown (Game2048 *game);

/* Sinh ô mới (2 hoặc 4) tại vị trí trống ngẫu nhiên */
void Game_SpawnTile(Game2048 *game);

/* Kiểm tra trạng thái */
bool Game_CheckWin (Game2048 *game);
bool Game_CheckLose(Game2048 *game);

#endif /* INC_GAME2048_H_ */
