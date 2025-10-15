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
    bool check = false;
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
    PTOKEN arr_of_toks = tokenize_file(buff, file_size);
    // error handling
    if (NULL == arr_of_toks)
    {
        return ERROR;
    }

    check = line_format_checker(arr_of_toks);
    if (check == false)
    {
        printf("Invalid Line Format.\n");
        return ERROR;
    }

    // for line in lines
    // function to split line by words
    // do_tokenize

    //  function to interpret keywords from token
    // function to validate sentence structure is valid
    // function to evaluate valid line
    // iterate through freeing until you hit EOF token
    for (int i = 0;; i++)
    {
        if (arr_of_toks[i].lexeme != NULL)
        {
            free(arr_of_toks[i].lexeme);
        }
        if (arr_of_toks[i].type == TOKEN_EOF)
        {
            break;
        }
    }
    free(arr_of_toks);
    free(buff);
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
    size_t bytes_read = fread(*buff, 1, file_size, file_contents);
    (*buff)[bytes_read] = '\0';
    fclose(file_contents);
    return bytes_read;
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
    // For Use with strtok_r with nested strtoks
    char *save_outer = NULL;
    char *save_inner = NULL;

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
        // Use strdup so they can be freed in the loop witht he rest of lexemes
        TOKEN eol = {0};
        eol.lexeme = strdup("\\n");
        eol.type = TOKEN_EOL;
        eol.token_num = index + 1;
        tok_array[index++] = eol;

        line = strtok_r(NULL, "\n", &save_outer);
    }
    TOKEN eof = {0};
    eof.lexeme = strdup("\\0");
    eof.type = TOKEN_EOF;
    eof.token_num = index + 1;
    tok_array[index++] = eof;

    for (int x = 0; x < index; x++)
    {
        printf("Array Data:\nToken Num: %d\nToken Type: %s\nLexeme: %s\n\n", tok_array[x].token_num, get_token_type_name(tok_array[x].type), tok_array[x].lexeme);
    }

    free(temp);

    return tok_array;
}

TOKEN analyze_word(char *word)
{
    TOKEN tok;
    tok.lexeme = strdup(word);
    tok.type = TOKEN_UNKNOWN;

    if (is_keyword(word))
    {
        tok.type = TOKEN_KEYWORD;
    }
    else if (is_all_digits(word))
    {
        tok.type = TOKEN_INT;
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
    }
    else if (is_math_symbol(word))
    {
        if (strcmp(word, "=") == 0)
        {
            tok.type = TOKEN_EQUALS;
        }
        else if (strcmp(word, "+") == 0)
        {
            tok.type = TOKEN_ADDITION;
        }
        else if (strcmp(word, "-") == 0)
        {
            tok.type = TOKEN_SUBTRACTION;
        }
        else if (strcmp(word, "*") == 0)
        {
            tok.type = TOKEN_MULTIPLICATION;
        }
        else if (strcmp(word, "/") == 0)
        {
            tok.type = TOKEN_DIVISION;
        }
    }
    else
    {
        tok.type = TOKEN_VARNAME;
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

bool line_format_checker(PTOKEN arr_of_toks)
{
    /*
    iterate through array
    while not null or tok_EOL
    add tok[i].type to temp sentence structure array.
    when reach EOL or EOF compare array against valid formats based on keyword
    */
    /*
    vars:
    Char sentence buffer[];  to append to
    counter (maybe need token count?)
    struct array of valid sentence structures? maybe split by KW?
    */
    int i = 0, counter = 0;
    const char **sentence_structure;
    const char *valid_patterns[] = {
        "TOKEN_INT TOKEN_KEYWORD TOKEN_VARNAME TOKEN_EQUALS TOKEN_INT",
        "TOKEN_INT TOKEN_KEYWORD TOKEN_VARNAME TOKEN_EQUALS TOKEN_VARNAME",
        "TOKEN_INT TOKEN_KEYWORD TOKEN_VARNAME TOKEN_EQUALS TOKEN_VARNAME TOKEN_ARITHMETIC TOKEN_INT",
        "TOKEN_INT TOKEN_KEYWORD TOKEN_VARNAME TOKEN_EQUALS TOKEN_INT TOKEN_ARITHMETIC TOKEN_INT",
        NULL};

    if (NULL == arr_of_toks)
    {
        printf("NULL array recvd.\n");
        return false;
    }

    while (arr_of_toks[i].type != TOKEN_EOL && arr_of_toks[i].type != TOKEN_EOF)
    {
        // get sentence length
        counter++;
        i++;
    }

    // malloc char* for that size
    sentence_structure = malloc(counter * sizeof(char *));
    if (NULL == sentence_structure)
    {
        perror("Malloc Failed");
        return false;
    }

    // Reset counter
    i = 0;

    while (arr_of_toks[i].type != TOKEN_EOL && arr_of_toks[i].type != TOKEN_EOF)
    {
        // add stuff to it
        sentence_structure[i] = get_token_type_name(arr_of_toks[i].type);
        i++;
    }

    // mem mgmt for building pattern to match against
    size_t total_len = 0;
    for (int j = 0; j < counter; j++)
    {
        // Add each word + space
        total_len += strlen(sentence_structure[j]) + 1;
    }

    // +1 for null term
    char *pattern_str = malloc(total_len + 1);
    if (NULL == pattern_str)
    {
        perror("Pattern malloc failed");
        free(sentence_structure);
        return false;
    }

    pattern_str[0] = '\0';
    for (int j = 0; j < counter; j++)
    {
        // copy each token into the string
        strcat(pattern_str, sentence_structure[j]);
        if (j < counter - 1)
        {
            strcat(pattern_str, " ");
        }
    }

    printf("Built pattern: [%s]\n", pattern_str);

    bool match_found = false;
    int matched_index = -1;

    // compare sentence structure against
    for (int k = 0; valid_patterns[k] != NULL; k++)
    {
        if (strcmp(pattern_str, valid_patterns[k]) == 0)
        {

            printf("Valid pattern found: %s\n", valid_patterns[k]);
            matched_index = k;
            match_found = true;
            break;
        }
    }

    if (match_found)
    {
        printf("Pattern matched.\n");
        switch (matched_index)
        {
        case 0:
            // char *endptr = NULL;
            // char *varname = arr_of_toks[2].lexeme;
            // char *value_str = arr_of_toks[4].lexeme;
            // long value = strtol(value_str, &endptr, 10);
            // // How to do assignment under the hood?
            // int varname = value;
            // printf("Set [%s] to [%ld]\n", varname, value);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            printf("Unandled Pattern.\n");
            break;
        }
        // Do something depending on which one matched.
        return true;
    }
    else
    {
        printf("Invalid pattern.\n");
        return false;
    }
    free(pattern_str);
    free(sentence_structure);
    return true;
}