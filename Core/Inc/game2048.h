#ifndef INC_GAME2048_H_
#define INC_GAME2048_H_
#include <stdint.h>
#include <stdbool.h>
#define GRID_SIZE 4
typedef struct {
    uint16_t board[GRID_SIZE][GRID_SIZE]; // Giá trị từng ô (0 = trống)
    uint32_t score;                       // Điểm số hiện tại
    bool     won;                         // Đã đạt 2048?
    bool     over;                        // Đã hết nước đi?
} Game2048;
void Game_Init(Game2048 *game);                 // Reset bàn cờ
bool Game_MoveUp(Game2048 *game);               // Trả về true nếu bàn cờ thay đổi
bool Game_MoveDown(Game2048 *game);
bool Game_MoveLeft(Game2048 *game);
bool Game_MoveRight(Game2048 *game);
void Game_SpawnTile(Game2048 *game);            // Sinh số 2 hoặc 4 ở ô trống ngẫu nhiên
bool Game_CheckWin(Game2048 *game);
bool Game_CheckLose(Game2048 *game);
#endif
