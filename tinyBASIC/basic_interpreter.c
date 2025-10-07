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
            interpret_and_do_line(program_lines);
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
    while (curr)
    {
        tokenize_line(curr);
        curr = curr->next;
    }
    return head;
}

TOKEN analyze_word(char *word, int line_num)
{
    TOKEN tok;
    tok.lexeme = strdup(word);
    tok.line_num = line_num;
    tok.type = TOKEN_UNKNOWN;

    if (is_keyword(word))
    {
        tok.type = TOKEN_KEYWORD;
        tok.value.keyword = get_Keyword_Enum(word);
    }
    else if (is_all_digits(word))
    {
        tok.type = TOKEN_INT;
        tok.value.intValue = atoi(word);
    }
    else if (is_punctuation(word))
    {
        if (strcmp(word, "\"") == 0)
        {
            tok.type = TOKEN_QUOTATION;
        }
        else if (strcmp(word, ";") == 0)
        {
            tok.type = TOKEN_EOL;
        }
        else if (strcmp(word, ",") == 0)
        {
            tok.type = TOKEN_COMMA;
        }
        else
        {
            tok.type = TOKEN_PUNCTUATION;
        }
        tok.value.varName = word[0];
    }
    else if (is_math_symbol(word))
    {
        if (strcmp(word, "=") == 0)
        {
            tok.type = TOKEN_EQUALS;
        }
        else
        {
            tok.type = TOKEN_ARITHMETIC;
            if (strcmp(word, "+") == 0)
            {
                tok.value.operation = TOKEN_ADDITION;
            }
            else if (strcmp(word, "-") == 0)
            {
                tok.value.operation = TOKEN_SUBTRACTION;
            }
            else if (strcmp(word, "*") == 0)
            {
                tok.value.operation = TOKEN_MULTIPLICATION;
            }
            else if (strcmp(word, "/") == 0)
            {
                tok.value.operation = TOKEN_DIVISION;
            }
        }
    }
    else
    {
        tok.type = TOKEN_VARNAME;
        tok.value.varName = word[0];
    }

    return tok;
}

const char *get_token_type_name(TokenType type)
{
    if (type >= 0 && type < sizeof(token_names) / sizeof(token_names[0]) && token_names[type])
    {
        return token_names[type];
    }
    return "UNKNOWN_TOKEN";
}

const char *get_token_operation_name(OperationType type)
{
    if (type >= 0 && type < sizeof(operation_names) / sizeof(operation_names[0]) && operation_names[type])
    {
        return operation_names[type];
    }
    return "UNKNOWN_TOKEN";
}

int get_Keyword_Enum(char *word)
{
    if (strcmp(word, "LET") == 0)
    {
        return KW_LET;
    }
    if (strcmp(word, "PRINT") == 0)
    {
        return KW_PRINT;
    }
    return KW_UNKNOWN;
}

TOKEN key_word_check(char *word, int line_num)
{
    TOKEN curr_token;
    curr_token.type = TOKEN_UNKNOWN;
    curr_token.line_num = line_num;

    if (is_keyword(word))
    {
        curr_token.type = TOKEN_KEYWORD;
        curr_token.value.keyword = get_Keyword_Enum(word);
    }
    return curr_token;
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
    newNode->tokens = NULL;
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

void tokenize_line(node_t *node)
{
    if (!node || !node->line)
        return;

    char *line_copy = strdup(node->line); // duplicate so strtok doesn’t break original
    if (!line_copy)
        return;

    // Skip first token (line number)
    char *ptr = line_copy;
    while (*ptr && !isspace(*ptr))
        ptr++;
    while (*ptr && isspace(*ptr))
        ptr++;

    char *token_str = strtok(ptr, " ");
    while (token_str)
    {
        TOKEN tok = analyze_word(token_str, node->line_num);
        add_token_to_node(node, tok);
        token_str = strtok(NULL, " ");
    }

    free(line_copy);
}

void add_token_to_node(node_t *node, TOKEN token_info)
{
    if (NULL == node)
    {
        printf("NULL Node in Add to token.\n");
        return;
    }
    token_t *new_token = malloc(sizeof(token_t));
    if (!new_token)
    {
        printf("Memory allocation failed\n");
        return;
    }

    new_token->token = token_info;
    new_token->next = NULL;

    if (node->tokens == NULL)
    {
        node->tokens = new_token;
    }
    else
    {
        token_t *curr = node->tokens;
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = new_token;
    }
}

void interpret_and_do_line(node_t *head)
{
    node_t *curr = head;

    if (NULL == head)
    {
        printf("NULL Head in interpret and do lines.\n");
        return;
    }
    while (curr)
    {
        token_t *t = curr->tokens;
        while (t != NULL)
        {
            TOKEN tok = t->token;
            switch (tok.type)
            {
            case TOKEN_KEYWORD:
            {
                // Print statement for Keyword
                printf("%s(%s)\n", get_token_type_name(tok.type), get_keyword_name(tok.type));

                // Handling for Sentence based on keyword
                int i = 0;
                while (keyword_handlers[i].keyword != KW_EOF)
                {
                    if (keyword_handlers[i].keyword == t->token.value.keyword)
                    {
                        // Call the parsing function
                        keyword_handlers[i].parse_function(curr->tokens);
                        // Exit the inner loop after finding a match
                        break;
                    }
                    i++;
                }
                break;
            }
            case TOKEN_INT:
            {
                printf("%s(%d)\n", get_token_type_name(tok.type), tok.value.intValue);
                break;
            }
            case TOKEN_VARNAME:
            {
                printf("%s(%c)\n", get_token_type_name(tok.type), tok.value.varName);
                break;
            }
            case TOKEN_PUNCTUATION:
            {
                printf("%s(%c)\n", get_token_type_name(tok.type), tok.value.varName);
                break;
            }
            case TOKEN_QUOTATION:
            {
                printf("%s\n", get_token_type_name(tok.type));
                break;
            }
            case TOKEN_COMMA:
            {
                printf("%s\n", get_token_type_name(tok.type));
                break;
            }
            case TOKEN_EOL:
            {
                printf("%s\n", get_token_type_name(tok.type));
                break;
            }
            case TOKEN_ARITHMETIC:
            {
                const char *math_syms[] = {"+", "-", "*", "/"};
                printf("%s(%s)\n", get_token_operation_name(tok.value.operation), math_syms[tok.value.operation]);
                break;
            }
            case TOKEN_EQUALS:
            {
                printf("%s(=)\n", get_token_type_name(tok.type));
                break;
            }
            default:
            {
                printf("%s\n", get_token_type_name(tok.type));
                break;
            }
            }
            t = t->next;
        }
        curr = curr->next;
    }
}

const char *get_keyword_name(KeywordType kw)
{
    switch (kw)
    {
    case KW_LET:
    {
        return "LET";
    }
    case KW_PRINT:
    {
        return "PRINT";
    }
    default:
    {
        return "UNKNOWN";
    }
    }
}

void parse_let(token_t *stream)
{
    printf("Inside Parse Let\n");
}

void parse_print(token_t *stream)
{
    printf("Inside Parse Print\n");
}