#include "game2048.h"
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------
 * Hàm nội bộ: slide + merge 1 mảng 4 phần tử về bên trái
 * Trả về true nếu mảng thay đổi
 * ------------------------------------------------------- */
static bool slide_and_merge(uint16_t line[GRID_SIZE], uint32_t *score)
{
    uint16_t orig[GRID_SIZE];
    memcpy(orig, line, sizeof(orig));

    /* Bước 1: dồn khác 0 về đầu */
    uint16_t tmp[GRID_SIZE] = {0};
    int pos = 0;
    for (int i = 0; i < GRID_SIZE; i++)
        if (line[i]) tmp[pos++] = line[i];

    /* Bước 2: gộp cặp liền kề bằng nhau */
    for (int i = 0; i < GRID_SIZE - 1; i++) {
        if (tmp[i] && tmp[i] == tmp[i+1]) {
            tmp[i] *= 2;
            *score  += tmp[i];
            tmp[i+1] = 0;
            i++;           /* bỏ qua ô vừa bị gộp */
        }
    }

    /* Bước 3: dồn lại lần 2 */
    memset(line, 0, GRID_SIZE * sizeof(uint16_t));
    pos = 0;
    for (int i = 0; i < GRID_SIZE; i++)
        if (tmp[i]) line[pos++] = tmp[i];

    return memcmp(orig, line, sizeof(orig)) != 0;
}

/* -------------------------------------------------------
 * Hàm public
 * ------------------------------------------------------- */
void Game_Init(Game2048 *game)
{
    memset(game->board, 0, sizeof(game->board));
    game->score = 0;
    game->won   = false;
    game->over  = false;
    Game_SpawnTile(game);
    Game_SpawnTile(game);
}

void Game_SpawnTile(Game2048 *game)
{
    uint8_t empty[16][2];
    int cnt = 0;
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (!game->board[r][c]) {
                empty[cnt][0] = r;
                empty[cnt][1] = c;
                cnt++;
            }
    if (!cnt) return;
    int idx = rand() % cnt;
    game->board[empty[idx][0]][empty[idx][1]] = (rand() % 10 < 9) ? 2 : 4;
}

bool Game_MoveLeft(Game2048 *game)
{
    bool moved = false;
    for (int r = 0; r < GRID_SIZE; r++)
        if (slide_and_merge(game->board[r], &game->score))
            moved = true;
    return moved;
}

bool Game_MoveRight(Game2048 *game)
{
    bool moved = false;
    for (int r = 0; r < GRID_SIZE; r++) {
        uint16_t tmp[GRID_SIZE];
        for (int c = 0; c < GRID_SIZE; c++)
            tmp[c] = game->board[r][GRID_SIZE-1-c];
        if (slide_and_merge(tmp, &game->score)) {
            moved = true;
            for (int c = 0; c < GRID_SIZE; c++)
                game->board[r][GRID_SIZE-1-c] = tmp[c];
        }
    }
    return moved;
}

bool Game_MoveUp(Game2048 *game)
{
    bool moved = false;
    for (int c = 0; c < GRID_SIZE; c++) {
        uint16_t tmp[GRID_SIZE];
        for (int r = 0; r < GRID_SIZE; r++) tmp[r] = game->board[r][c];
        if (slide_and_merge(tmp, &game->score)) {
            moved = true;
            for (int r = 0; r < GRID_SIZE; r++) game->board[r][c] = tmp[r];
        }
    }
    return moved;
}

bool Game_MoveDown(Game2048 *game)
{
    bool moved = false;
    for (int c = 0; c < GRID_SIZE; c++) {
        uint16_t tmp[GRID_SIZE];
        for (int r = 0; r < GRID_SIZE; r++) tmp[r] = game->board[GRID_SIZE-1-r][c];
        if (slide_and_merge(tmp, &game->score)) {
            moved = true;
            for (int r = 0; r < GRID_SIZE; r++) game->board[GRID_SIZE-1-r][c] = tmp[r];
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
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++) {
            if (!game->board[r][c]) return false;
            if (c < GRID_SIZE-1 && game->board[r][c] == game->board[r][c+1]) return false;
            if (r < GRID_SIZE-1 && game->board[r][c] == game->board[r+1][c]) return false;
        }
    return true;
}
