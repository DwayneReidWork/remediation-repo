#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BOARD_ROW 5
#define BOARD_COL 5

typedef struct
{
    uint16_t board[BOARD_ROW][BOARD_COL];
} Game;

void print_board(Game *game_inst);
void populate_board(Game *game_inst, char any);

int main()
{
    Game *game = malloc(sizeof(Game));
    if (!game)
    {
        perror("Failed to allocate memory");
        return 1;
    }
    populate_board(game, 'S');
    print_board(game);
    return 0;
}

/// @brief Given ptr to struct and a char will generate a 5x5 board of the char
/// @param game ptr to game struct
/// @param whatever a single char
void populate_board(Game *game, char whatever)
{
    for (int i = 0; i < BOARD_ROW; i++)
    {
        for (int j = 0; j < BOARD_COL; j++)
        {
            game->board[i][j] = whatever;
        }
    }
}

/// @brief displays board
/// @param game_inst pointer to game struct
void print_board(Game *game_inst)
{
    printf("    1    2    3    4    5\n");
    for (int i = 0; i < BOARD_ROW; i++)
    {
        printf("%d ", i + 1);
        for (int j = 0; j < BOARD_COL; j++)
        {
            printf("  %c  ", game_inst->board[i][j]);
        }
        printf("\n");
    }
}