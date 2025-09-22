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
<<<<<<< HEAD

int main()
{
=======
void update_board(Game *game, int row, int col);

int main()
{
    int row = 2;
    int col = 2;

>>>>>>> Mem_Mgmt_and_File_IO
    Game *game = malloc(sizeof(Game));
    if (!game)
    {
        perror("Failed to allocate memory");
        return 1;
    }
<<<<<<< HEAD
    char filler = 'X';
    populate_board(game, filler);
    print_board(game);
=======
    char filler = '.';
    populate_board(game, filler);
    print_board(game);
    update_board(game, row, col);
    print_board(game);

    free(game);
>>>>>>> Mem_Mgmt_and_File_IO
    return 0;
}

/// @brief Given ptr to struct and a char will generate a 5x5 board of the char
/// @param game ptr to game struct
/// @param whatever a single char
void populate_board(Game *game, char filler)
{
    for (int i = 0; i < BOARD_ROW; i++)
    {
        for (int j = 0; j < BOARD_COL; j++)
        {
            game->board[i][j] = filler;
        }
    }
}

/// @brief displays board
/// @param game_inst pointer to game struct
void print_board(Game *game_inst)
{
<<<<<<< HEAD
    printf("    1    2    3    4    5\n");
=======
    printf("\n    1    2    3    4    5\n");
>>>>>>> Mem_Mgmt_and_File_IO
    for (int i = 0; i < BOARD_ROW; i++)
    {
        printf("%d ", i + 1);
        for (int j = 0; j < BOARD_COL; j++)
        {
            printf("  %c  ", game_inst->board[i][j]);
        }
        printf("\n");
    }
<<<<<<< HEAD
=======
}

void update_board(Game *game, int row, int col)
{
    if (NULL == game || 0 > row || 0 > col || 5 < row || 5 < col)
    {
        printf("Invalid input received\n");
        return;
    }

    // How to deal with 0 indexing?
    // Why cant I do this?
    //  row - 1 = row;
    //  col - 1 = col;

    game->board[row - 1][col - 1] = 'X';
    game->board[row][col] = 'X';
>>>>>>> Mem_Mgmt_and_File_IO
}