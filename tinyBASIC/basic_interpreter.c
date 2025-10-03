#include <ctype.h>
#include <stdbool.h>
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

    while (curr != NULL)
    {
        tokenize_line(curr->line);
        curr = curr->next;
    }

    // Figure out proper return
    return head;
}

int analyze_word(char *word)
{
    TOKEN curr_token;

    // Find Numbers
    if (is_all_digits(word))
    {
        curr_token.type = TOKEN_INT;
        curr_token.value.intValue = atoi(word);
        printf("%s(%d)\n", get_token_type_name(curr_token.type), curr_token.value.intValue);
    }
    else if (is_punctuation(word))
    {
        curr_token.type = TOKEN_PUNCTUATION;
        curr_token.value.varName = word[0];
        printf("%s(%c)\n", get_token_type_name(curr_token.type), curr_token.value.varName);
    }
    else if (is_math_symbol(word))
    {
        if (strcmp(word, "=") == 0)
        {
            curr_token.type = TOKEN_EQUALS;
        }
        else
        {
            curr_token.type = TOKEN_ARITHMETIC;
        }
        curr_token.value.varName = word[0];
        printf("%s(%c)\n", get_token_type_name(curr_token.type), curr_token.value.varName);
    }
    else if (is_keyword(word))
    {
        curr_token.type = TOKEN_KEYWORD;
        printf("%s(%s)\n", get_token_type_name(curr_token.type), word);
    }
    else
    {
        curr_token.type = TOKEN_VARNAME;
        printf("%s(%s)\n", get_token_type_name(curr_token.type), word);
    }
    return 0;
}

const char *get_token_type_name(TokenType type)
{
    switch (type)
    {
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
    case TOKEN_VARNAME:
        return "TOKEN_VARNAME";
    case TOKEN_PUNCTUATION:
        return "TOKEN_PUNCTUATION";
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_ARITHMETIC:
        return "TOKEN_ARITHMETIC";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_EOL:
        return "TOKEN_EOL";
    case TOKEN_UNKNOWN:
        return "TOKEN_UNKNOWN";
    default:
        return "UNKNOWN_TYPE";
    }
}

bool is_all_digits(char *word)
{
    for (int i = 0; word[i]; i++)
    {
        if (!isdigit(word[i]))
            return false;
    }
    return true;
}

bool is_punctuation(char *word)
{
    const char *punctuation[] = {",", "\"", ";"};
    size_t punctuation_size = sizeof(punctuation) / sizeof(punctuation[0]);
    for (int i = 0; i < punctuation_size; i++)
    {
        if (strcmp(word, punctuation[i]) == 0)
            return true;
    }
    return false;
}

bool is_math_symbol(char *word)
{
    const char *math_syms[] = {"+", "-", "*", "/", "="};
    size_t math_syms_size = sizeof(math_syms) / sizeof(math_syms[0]);
    for (int i = 0; i < math_syms_size; i++)
    {
        if (strcmp(word, math_syms[i]) == 0)
            return true;
    }
    return false;
}

bool is_keyword(char *word)
{
    const char *reservedKeyWords[] = {"LET", "PRINT", "GO TO", "GOSUB", "RETURN", "INPUT"};

    size_t keyword_size = sizeof(reservedKeyWords) / sizeof(reservedKeyWords[0]);

    for (int i = 0; i < keyword_size; i++)
    {
        if (strcmp(word, reservedKeyWords[i]) == 0)
            return true;
    }
    return false;
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
    char *word = NULL;

    while (*line != '\0')
    {
        while (*line == ' ')
        {
            line++;
        }

        if (*line == '\0')
        {
            break;
        }

        if (ispunct(*line))
        {
            // Char + null term
            word = malloc(sizeof(char) * 2);
            if (word == NULL)
            {
                printf("Memory allocation failed\n");
                return;
            }
            word[0] = *line;
            word[1] = '\0';
            analyze_word(word);
            line++;
        }
        else
        {
            char *start = line;
            while (*line != ' ' && *line != '\0')
            {
                line++;
            }

            size_t len = line - start;
            word = malloc(len + 1);
            if (word == NULL)
            {
                printf("Memory allocation failed\n");
                return;
            }

            strncpy(word, start, len);
            word[len] = '\0';

            analyze_word(word);

            free(word);
        }
    }
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// void tokenize_line(char *line)
// {
//     char *ptr = line;

//     while (*ptr != '\0')
//     {
//         // Skip whitespace
//         while (*ptr == ' ' || *ptr == '\t')
//             ptr++;

//         if (*ptr == '\0')
//             break;

//         if (isalpha(*ptr)) // Keyword or var name
//         {
//             char word[64];
//             int i = 0;

//             while (isalnum(*ptr) && i < 63)
//             {
//                 word[i++] = *ptr;
//                 ptr++;
//             }
//             word[i] = '\0';

//             // Check if word is keyword
//             if (strcmp(word, "LET") == 0)
//             {
//                 printf("TOKEN_KEYWORD(LET)\n");
//             }
//             else if (strcmp(word, "PRINT") == 0)
//             {
//                 printf("TOKEN_KEYWORD(PRINT)\n");
//             }
//             else
//             {
//                 printf("TOKEN_VARNAME(%s)\n", word);
//             }
//         }
//         else if (isdigit(*ptr))
//         {
//             char number[32];
//             int i = 0;

//             while (isdigit(*ptr) && i < 31)
//             {
//                 number[i++] = *ptr;
//                 ptr++;
//             }
//             number[i] = '\0';

//             printf("TOKEN_NUMBER(%s)\n", number);
//         }
//         else if (*ptr == '=')
//         {
//             printf("TOKEN_EQUALS\n");
//             ptr++;
//         }
//         else if (*ptr == '+')
//         {
//             printf("TOKEN_ARITHMETIC(+)\n");
//             ptr++;
//         }
//         else if (*ptr == '-')
//         {
//             printf("TOKEN_ARITHMETIC(-)\n");
//             ptr++;
//         }
//         else if (*ptr == '*')
//         {
//             printf("TOKEN_ARITHMETIC(*)\n");
//             ptr++;
//         }
//         else if (*ptr == '/')
//         {
//             printf("TOKEN_ARITHMETIC(/)\n");
//             ptr++;
//         }
//         else
//         {
//             // Unknown char
//             printf("TOKEN_UNKNOWN(%c)\n", *ptr);
//             ptr++;
//         }
//     }

//     printf("TOKEN_EOL\n");
// }

// WILL THIS BE REUSED ELSEWHERE? TBD
//  switch (cmd)
//  {
//  case TOKEN_PRINT:
//      printf("Recieved token: %d\n", cmd);
//      break;
//  case TOKEN_LET:
//      printf("Recieved token: %d\n", cmd);
//      break;
//  case TOKEN_GO_TO:
//      printf("Recieved token: %d\n", cmd);
//      break;
//  case TOKEN_IF:
//      printf("Recieved token: %d\n", cmd);
//      break;
//  case TOKEN_GOSUB:
//      printf("Recieved token: %d\n", cmd);
//      break;
//  case TOKEN_RETURN:
//      printf("Recieved token: %d\n", cmd);
//      break;
//  case TOKEN_UNKNOWN:
//      break;
//  default:
//      printf("Hit default case\n");
//      break;