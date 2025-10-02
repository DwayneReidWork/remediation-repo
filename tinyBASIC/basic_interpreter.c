#include <ctype.h>
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
            parse_node_word_by_word(program_lines);
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

node_t *parse_node_word_by_word(node_t *head)
{
    node_t *curr = head;
    TOKEN current_token;
    char *token;
    current_token.type = TOKEN_UNKNOWN;

    const char *reservedKeyWords[] = {"LET", "PRINT", "GO TO", "GOSUB", "RETURN", "INPUT"};

    while (curr->next != NULL)
    {
        tokenize_line(curr->line);
        curr = curr->next;
    }

    // Figure out proper return
    return head;
}

int act_on_cmd(TokenType cmd, char *data)
{
    switch (cmd)
    {
    case TOKEN_PRINT:
        printf("Recieved token: %d\n", cmd);
        break;
    case TOKEN_LET:
        printf("Recieved token: %d\n", cmd);
        break;
    case TOKEN_GO_TO:
        printf("Recieved token: %d\n", cmd);
        break;
    case TOKEN_IF:
        printf("Recieved token: %d\n", cmd);
        break;
    case TOKEN_GOSUB:
        printf("Recieved token: %d\n", cmd);
        break;
    case TOKEN_RETURN:
        printf("Recieved token: %d\n", cmd);
        break;
    case TOKEN_UNKNOWN:
        break;
    default:
        printf("Hit default case\n");
        break;
    }

    return 0;
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

void tokenize_line(char *line)
{
    char *ptr = line;

    while (*ptr != '\0')
    {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t')
            ptr++;

        if (*ptr == '\0')
            break;

        if (isalpha(*ptr)) // Keyword or var name
        {
            char word[64];
            int i = 0;

            while (isalnum(*ptr) && i < 63)
            {
                word[i++] = *ptr;
                ptr++;
            }
            word[i] = '\0';

            // Check if word is keyword
            if (strcmp(word, "LET") == 0)
            {
                printf("TOKEN_KEYWORD(LET)\n");
            }
            else if (strcmp(word, "PRINT") == 0)
            {
                printf("TOKEN_KEYWORD(PRINT)\n");
            }
            else
            {
                printf("TOKEN_VARNAME(%s)\n", word);
            }
        }
        else if (isdigit(*ptr))
        {
            char number[32];
            int i = 0;

            while (isdigit(*ptr) && i < 31)
            {
                number[i++] = *ptr;
                ptr++;
            }
            number[i] = '\0';

            printf("TOKEN_NUMBER(%s)\n", number);
        }
        else if (*ptr == '=')
        {
            printf("TOKEN_EQUALS\n");
            ptr++;
        }
        else if (*ptr == '+')
        {
            printf("TOKEN_ARITHMETIC(+)\n");
            ptr++;
        }
        else if (*ptr == '-')
        {
            printf("TOKEN_ARITHMETIC(-)\n");
            ptr++;
        }
        else if (*ptr == '*')
        {
            printf("TOKEN_ARITHMETIC(*)\n");
            ptr++;
        }
        else if (*ptr == '/')
        {
            printf("TOKEN_ARITHMETIC(/)\n");
            ptr++;
        }
        else
        {
            // Unknown char
            printf("TOKEN_UNKNOWN(%c)\n", *ptr);
            ptr++;
        }
    }

    printf("TOKEN_EOL\n");
}