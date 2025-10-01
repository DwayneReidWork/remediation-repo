#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic_interpreter.h"

int main(int argc, char *argv[])
{
    char *contents = NULL;

    if (argc > 2)
    {
        printf("Usage: %s for interactive shell\n%s <.bas filename> to execute file.\n", argv[0], argv[0]);
    }
    else if (argc == 2)
    {
        contents = load_file_contents(argv[1]);
        if (contents)
        {
            node_t *program_lines = parse_lines_to_nodes(contents);
            parse_node_for_cmds(program_lines);
            free(program_lines);
        }
        free(contents);
    }
    else if (argc < 2)
    {
        // Open live interactive shell.
        printf("Open Shell.\n");
    }
}

char *load_file_contents(char *filename)
{
    if (filename == NULL)
    {
        printf("Invalid filename.\n");
        return NULL;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char *buffer = (char *)malloc(filesize + 1);
    if (buffer == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(fp);
        return NULL;
    }

    fread(buffer, 1, filesize, fp);
    buffer[filesize] = '\0';

    fclose(fp);
    return buffer;
}

/// @brief split file into nodes by line
/// @param file_contents ptr to loaded file contents
/// @return pointer to head
node_t *parse_lines_to_nodes(char *file_contents)
{
    node_t *head = NULL;
    int line_num = 1;

    // Split into lines by newline
    char *line = strtok(file_contents, "\n");
    while (line != NULL)
    {
        node_t *new_node = create_new_node(line_num, line);
        insertAtEnd(&head, new_node);
        line = strtok(NULL, "\n");
        line_num++;
    }

    return head;
}

node_t *parse_node_for_cmds(node_t *head)
{
    node_t *curr = head;
    char *token;
    while (curr->next != NULL)
    {
        printf("Current Id: %d\n", curr->line_num);
        token = strtok(curr->line, " ");
        printf("Line number: %s\n", token);

        token = strtok(NULL, " ");
        printf("Command: %s\n", token);

        // get rest of the line including spaces
        token = strtok(NULL, "");
        printf("Argument: %s\n", token);
        curr = curr->next;
    }
    // Figure out proper return
    return head;
}

node_t *create_new_node(int line_num, char *line)
{
    node_t *newNode = malloc(sizeof(node_t));
    newNode->line_num = line_num;
    newNode->line = line;

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

void insertAtEnd(node_t **head, node_t *new_node)
{
    // check if DLL is empty
    if (*head == NULL)
    {
        *head = new_node;
        return;
    }

    node_t *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->prev = temp;
}

void free_list(node_t *head)
{
    node_t *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }
}