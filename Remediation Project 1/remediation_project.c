#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_SIZE 10
#define COL_SIZE 10

typedef struct
{
    char *name;
    u_int32_t name_len;
    u_int16_t board[ROW_SIZE][COL_SIZE];
    bool game_over;
} mock_struct;

int load_data(mock_struct *ms_ptr);
int save_data(mock_struct *ms_ptr);
void fill_struct(mock_struct *ms_ptr);

int main()
{

    mock_struct *instance_of_struct = malloc(sizeof(mock_struct));
    mock_struct *loaded_struct = malloc(sizeof(mock_struct));

    if (NULL == instance_of_struct)
    {
        printf("Malloc failed");
        return -1;
    }

    if (NULL == loaded_struct)
    {
        printf("Malloc failed");
        return -1;
    }

    printf("Now filling struct\n");
    fill_struct(instance_of_struct);

    printf("Now saving struct\n");
    save_data(instance_of_struct);

    printf("Now loading data\n");
    load_data(loaded_struct);

    free(loaded_struct->name);
    free(instance_of_struct->name);
    free(instance_of_struct);
    free(loaded_struct);

    return 0;
}

/// @brief Populate struct with initial mock data
/// @param ms_ptr pointer to mock struct
/// @return nothing
void fill_struct(mock_struct *ms_ptr)
{
    char *name = "Dwayne";
    ms_ptr->name_len = strlen(name);

    ms_ptr->name = malloc(ms_ptr->name_len + 1);
    strncpy(ms_ptr->name, name, strlen(name));
    ms_ptr->name[ms_ptr->name_len] = '\0';

    ms_ptr->game_over = false;

    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            // populate board with just periods (for now)
            ms_ptr->board[i][j] = '.';
        }
    }
}

/// @brief Load data from file into struct
/// @param ms_ptr pointer to mock struct
/// @return 0 on success -1 on fail (should return ptr to struct?)
int load_data(mock_struct *ms_ptr)
{
    if (NULL == ms_ptr)
    {
        printf("Struct ptr is null.\n");
        return -1;
    }

    FILE *load_file = fopen("save_file", "rb");

    if (NULL == load_file)
    {
        printf("Error opening file\n");
        return -1;
    }
    // Read Name Len
    fread(&ms_ptr->name_len, sizeof(ms_ptr->name_len), 1, load_file);
    ms_ptr->name = malloc(ms_ptr->name_len + 1);
    // Read Name
    fread(ms_ptr->name, sizeof(char), ms_ptr->name_len, load_file);
    ms_ptr->name[ms_ptr->name_len] = '\0';
    // Game Status
    fread(&ms_ptr->game_over, sizeof(ms_ptr->game_over), 1, load_file);
    // Board
    fread(ms_ptr->board, sizeof(ms_ptr->board), 1, load_file);

    printf("Loaded File Contents\nName: %s\nGame Over Status: %s\n", ms_ptr->name, ms_ptr->game_over ? "true" : "false");

    printf("Board\n");
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COL_SIZE; j++)
        {
            printf("%c", (char)ms_ptr->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    fclose(load_file);
    return 0;
};

/// @brief Saves data to a file
/// @param ms_ptr pointer to mock struct
/// @return -1 on fail 0 on succcess
int save_data(mock_struct *ms_ptr)
{
    if (NULL == ms_ptr)
    {
        printf("Struct ptr is null.\n");
        return -1;
    }

    FILE *save_file = fopen("save_file", "wb");

    if (NULL == save_file)
    {
        printf("Error creating/opening file\n");
        return -1;
    }

    // Name Len
    fwrite(&ms_ptr->name_len, sizeof(ms_ptr->name_len), 1, save_file);
    // Name
    fwrite(ms_ptr->name, sizeof(char), ms_ptr->name_len, save_file);
    // Game status (bool)
    fwrite(&ms_ptr->game_over, sizeof(ms_ptr->game_over), 1, save_file);
    // Board
    fwrite(ms_ptr->board, sizeof(ms_ptr->board), 1, save_file);

    // Close ptr
    fclose(save_file);
    return 0;
}
