// save load w/regex
// nested loops

// create a save file with unix timestamp. (1 hr 3 mins)
// Load file and print text in file.

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME_SIZE 1024

typedef struct
{
    char *username;
    uint32_t username_len;
} Player;

int save_data(Player *p_ptr);
int load_data(Player *p_ptr);
void fill_struct(Player *p_ptr);

int main()
{
    Player *player1 = malloc(sizeof(Player));
    Player loaded_player_data;
    printf("Filling Data\n");
    fill_struct(player1);
    printf("Saving Data\n");
    save_data(player1);
    printf("Loading Data\n");
    load_data(&loaded_player_data);
}

int save_data(Player *p_ptr)
{
    if (NULL == p_ptr)
    {
        printf("Recieved null pointer\n");
        return -1;
    }

    char save_name[MAX_FILENAME_SIZE];
    char timestamp_str[32];
    time_t timestamp = time(NULL);

    // convert time stamp to string
    snprintf(timestamp_str, sizeof(timestamp_str), "%ld", (long)timestamp);

    if (p_ptr->username_len + strlen(timestamp_str) + 1 > MAX_FILENAME_SIZE)
    {
        printf("Error: Filename buffer is too small.\n");
        return 1;
    }
    // merger username and time
    snprintf(save_name, sizeof(save_name), "%s%ld", p_ptr->username, (long)timestamp);

    printf("User: %s\n", save_name);

    FILE *savefp = fopen(save_name, "wb");

    if (NULL == savefp)
    {
        printf("Error creating/opening file.\n");
        return -1;
    }

    fwrite(&p_ptr->username_len, sizeof(p_ptr->username_len), 1, savefp);
    fwrite(p_ptr->username, sizeof(char), p_ptr->username_len, savefp);

    return 0;
}

int load_data(Player *p_ptr)
{
    if (NULL == p_ptr)
    {
        printf("Recieved null pointer\n");
        return -1;
    }

    DIR *dir_ptr;
    struct dirent *dir_entry;
    const char *directory_path = ".";
    dir_ptr = opendir(directory_path);

    if (NULL == dir_ptr)
    {
        printf("Error opening directory\n");
        return -1;
    }

    while (NULL != (dir_entry = readdir(dir_ptr)))
    {
        if (strstr(dir_entry->d_name, p_ptr->username) != NULL)
        {
            char filepath[MAX_FILENAME_SIZE];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory_path, dir_entry->d_name);

            FILE *load_file = fopen(filepath, "rb");
            if (!load_file)
            {
                printf("Error opening file.");
                return -1;
            }

            // Name Len
            fread(&p_ptr->username_len, sizeof(p_ptr->username_len), 1, load_file);
            p_ptr->username = malloc(p_ptr->username_len + 1);

            // Name
            fread(p_ptr->username, sizeof(char), p_ptr->username_len, load_file);
            p_ptr->username[p_ptr->username_len] = '\0';
        }
    }

    if (closedir(dir_ptr) == -1)
    {
        printf("Error closing directory");
        return -1;
    }
    return 0;
}

/// @brief Populate struct with initial mock data
/// @param p_ptr pointer to mock struct
/// @return nothing
void fill_struct(Player *p_ptr)
{
    char *name = "Dwayne";
    p_ptr->username_len = strlen(name);

    p_ptr->username = malloc(p_ptr->username_len + 1);
    strncpy(p_ptr->username, name, strlen(name));
    p_ptr->username[p_ptr->username_len] = '\0';
}
