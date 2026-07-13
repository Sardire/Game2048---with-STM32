#include "game2048.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
// --- Hàm nội bộ ---
// Xử lý trượt + gộp cho 1 hàng/cột (mảng 4 phần tử), hướng sang trái
// Trả về true nếu mảng thay đổi
static bool slide_and_merge(uint16_t line[GRID_SIZE], uint32_t *score)
{
    uint16_t original[GRID_SIZE];
    memcpy(original, line, sizeof(original));
    // Bước 1: Dồn các số khác 0 về đầu mảng (bỏ khoảng trống)
    uint16_t temp[GRID_SIZE] = {0};
    int pos = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        if (line[i] != 0) {
            temp[pos++] = line[i];
        }
    }
    // Bước 2: Gộp các cặp giống nhau liền kề
    for (int i = 0; i < GRID_SIZE - 1; i++) {
        if (temp[i] != 0 && temp[i] == temp[i + 1]) {
            temp[i] *= 2;
            *score += temp[i];  // Cộng điểm
            temp[i + 1] = 0;   // Ô bên phải bị "ăn"
            i++;                // Bỏ qua ô vừa bị gộp (tránh gộp dây chuyền)
        }
    }
    // Bước 3: Dồn lại lần nữa
    memset(line, 0, GRID_SIZE * sizeof(uint16_t));
    pos = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        if (temp[i] != 0) {
            line[pos++] = temp[i];
        }
    }
    // Kiểm tra mảng có thay đổi không
    return memcmp(original, line, sizeof(original)) != 0;
}
// --- Hàm public ---
void Game_Init(Game2048 *game)
{
    memset(game->board, 0, sizeof(game->board));
    game->score = 0;
    game->won = false;
    game->over = false;
    Game_SpawnTile(game);
    Game_SpawnTile(game);
}
void Game_SpawnTile(Game2048 *game)
{
    // Tìm tất cả ô trống
    uint8_t empty_cells[16][2];
    int count = 0;
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (game->board[r][c] == 0) {
                empty_cells[count][0] = r;
                empty_cells[count][1] = c;
                count++;
            }
        }
    }
    if (count == 0) return;
    // Chọn ngẫu nhiên 1 ô trống, 90% sinh số 2, 10% sinh số 4
    int idx = rand() % count;
    game->board[empty_cells[idx][0]][empty_cells[idx][1]] = (rand() % 10 < 9) ? 2 : 4;
}
bool Game_MoveLeft(Game2048 *game)
{
    bool moved = false;
    for (int r = 0; r < GRID_SIZE; r++) {
        if (slide_and_merge(game->board[r], &game->score))
            moved = true;
    }
    return moved;
}
bool Game_MoveRight(Game2048 *game)
{
    bool moved = false;
    for (int r = 0; r < GRID_SIZE; r++) {
        // Đảo ngược hàng → slide trái → đảo lại
        uint16_t temp[GRID_SIZE];
        for (int c = 0; c < GRID_SIZE; c++)
            temp[c] = game->board[r][GRID_SIZE - 1 - c];
        if (slide_and_merge(temp, &game->score)) {
            moved = true;
            for (int c = 0; c < GRID_SIZE; c++)
                game->board[r][GRID_SIZE - 1 - c] = temp[c];
        }
    }
    return moved;
}
bool Game_MoveUp(Game2048 *game)
{
    bool moved = false;
    for (int c = 0; c < GRID_SIZE; c++) {
        // Trích cột thành mảng tạm → slide trái → ghi lại
        uint16_t temp[GRID_SIZE];
        for (int r = 0; r < GRID_SIZE; r++)
            temp[r] = game->board[r][c];
        if (slide_and_merge(temp, &game->score)) {
            moved = true;
            for (int r = 0; r < GRID_SIZE; r++)
                game->board[r][c] = temp[r];
        }
    }
    return moved;
}
bool Game_MoveDown(Game2048 *game)
{
    bool moved = false;
    for (int c = 0; c < GRID_SIZE; c++) {
        uint16_t temp[GRID_SIZE];
        for (int r = 0; r < GRID_SIZE; r++)
            temp[r] = game->board[GRID_SIZE - 1 - r][c];
        if (slide_and_merge(temp, &game->score)) {
            moved = true;
            for (int r = 0; r < GRID_SIZE; r++)
                game->board[GRID_SIZE - 1 - r][c] = temp[r];
        }
    }
    return moved;
}
bool Game_CheckWin(Game2048 *game)
{
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (game->board[r][c] == 2048) return true;
    return false;
}
bool Game_CheckLose(Game2048 *game)
{
    // Còn ô trống → chưa thua
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (game->board[r][c] == 0) return false;
    // Kiểm tra còn nước gộp được không
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++) {
            if (c < GRID_SIZE - 1 && game->board[r][c] == game->board[r][c+1]) return false;
            if (r < GRID_SIZE - 1 && game->board[r][c] == game->board[r+1][c]) return false;
        }
    return true; // Không còn nước nào → Thua
}
