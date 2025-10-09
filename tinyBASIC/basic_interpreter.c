#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "basic_interpreter.h"

int main(int argc, char *argv[])
{
    /*check arg count
    if 1 open shell
    if 2 open .bas file and parse
    if 3 throw error + usage
    */

    int status = SUCCESS;
    if (argc == 1)
    {
        // open shell
    }
    else if (argc == 2)
    {
        status = parse_file(argv[1]);
    }
    else
    {
        printf("Usage: %s to open shell.\n %s <filename.bas> to load and interpret file.\n", argv[0], argv[0]);
        return ERROR;
    }
    return status;
}

/// @brief Open file, load contents into LL,
/// @param filename
/// @return
int parse_file(char *filename)
{
    int status = SUCCESS;
    /*
    Open file
    get contents
    split contents on new line
    split line into tokens
    store tokens somewhere
    find keywords
    evaluate line by keyword
    */

    // some pointer to a buffer for the file
    // function to get file contents
    char *buff = NULL;

    long file_size = load_file(filename, &buff);
    // error handling
    if (-1 == file_size)
    {
        return ERROR;
    }

    // function to split file contents by line
    // status = split_buff_to_lines(buff, lines, &line_count, file_size);
    tokenize_file(buff, file_size);
    // error handling

    // for line in lines
    // function to split line by words
    // do_tokenize

    //  function to interpret keywords from token
    // function to validate sentence structure is valid
    // function to evaluate valid line

    return status;
}

long load_file(char *filename, char **buff)
{
    /*
    Read File
    populate buff w/file contents

    return file_len
    */

    if (NULL == filename)
    {
        printf("Error opening file.\n");
        return ERROR;
    }
    FILE *file_contents = fopen(filename, "r");
    if (NULL == file_contents)
    {
        perror("Failed to open file.");
        return ERROR;
    }

    fseek(file_contents, 0, SEEK_END);
    long file_size = ftell(file_contents);
    rewind(file_contents);

    *buff = malloc(file_size + 1);
    if (NULL == *buff)
    {
        perror("Malloc failed.");
        return ERROR;
    }
    fread(*buff, file_size, file_size, file_contents);
    fclose(file_contents);
    return file_size;
}

PTOKEN tokenize_file(char *buff, long file_size)
{
    /*
    given file tokenize word by word
    seperated by spaces/'\n'
    and new line chars
    append token to struct of tokens
    return struct
    */

    if (NULL == buff)
    {
        printf("Null buff recieved in tokenize file.\n");
        return NULL;
    }

    int tok_count = 0;
    int line_count = 0;

    for (int i = 0; i < file_size; i++)
    {
        if (buff[i] == '\n' || buff[i] == ' ')
        {
            tok_count++;
        }
        if (buff[i] == '\n')
        {
            line_count++;
        }
    }
    tok_count++;

    tok_count += line_count;

    printf("Tok Count: %d\n", tok_count);

    TOKEN *tok_array = calloc(tok_count, sizeof(TOKEN));
    if (NULL == tok_array)
    {
        perror("Tok Array Calloc Failed");
        return NULL;
    }

    char *temp = strdup(buff);

    if (temp == NULL)
    {
        perror("strdup failed");
        free(tok_array);
        return NULL;
    }

    int index = 0;
    char *save_outer;
    char *save_inner;

    char *line = strtok_r(temp, "\n", &save_outer);
    while (line != NULL && index < tok_count)
    {
        char *word = strtok_r(line, " ", &save_inner);
        while (word != NULL && index < tok_count)
        {
            TOKEN tok = analyze_word(word);
            tok.token_num = index + 1;
            tok_array[index++] = tok;
            word = strtok_r(NULL, " ", &save_inner);
        }
        TOKEN eol = {0};
        eol.lexeme = "\\n";
        eol.type = TOKEN_EOL;
        eol.token_num = index + 1;
        tok_array[index++] = eol;

        line = strtok_r(NULL, "\n", &save_outer);
    }

    for (int x = 0; x < index; x++)
    {
        printf("Array Data:\nToken Num: %d\nToken Type: %s\nLexeme: %s\n\n", tok_array[x].token_num, get_token_type_name(tok_array[x].type), tok_array[x].lexeme);
    }

    // free(temp);

    return tok_array;
}

// int split_buff_to_lines(char *buff, char **lines, int *line_count, long file_size)
// {
//     /*
//     taking buffer and splitting it on newline
//     adding that to lines
//     increment line count
//     */
//     // make sure this is not null first
//     *line_count = 1; // no matter what assume one line.
//     int iCurrentLineLen = 0;
//     int currentLine = 0;
//     char *left = buff;
//     char *right = left;

//     for (int i = 0; i < file_size; i++)
//     {
//         if (buff[i] == '\n')
//         {
//             (*line_count)++;
//         }
//     }

//     lines = calloc(*line_count, sizeof(char *));
//     // error check that
//     if (NULL == lines)
//     {
//         perror("Lines Calloc Failed.");
//         return ERROR;
//     }

//     while ('\0' != *left)
//     {
//         while (*right != '\n' && *right != '\0' && right < buff + file_size)
//         {
//             right++;
//         }

//         iCurrentLineLen = right - left + 1;
//         // allocate indv line
//         lines[currentLine] = calloc(iCurrentLineLen, sizeof(char));
//         if (lines[currentLine] == NULL)
//         {
//             perror("*Lines Calloc Failed");
//             return ERROR;
//         }
//         memcpy(lines[currentLine], left, iCurrentLineLen - 1);
//         printf("Line: %s\n", lines[currentLine]);

//         // move left pointer over to EOL
//         right++;
//         currentLine++;
//         left = right;
//     }
//     return 0;
// }

TOKEN analyze_word(char *word)
{
    TOKEN tok;
    tok.lexeme = strdup(word);
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

const char *get_token_type_name(TokenType type)
{
    if (type >= 0 && type < sizeof(token_names) / sizeof(token_names[0]) && token_names[type])
    {
        return token_names[type];
    }
    return "UNKNOWN_TOKEN";
}