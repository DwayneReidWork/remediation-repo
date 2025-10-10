#define SUCCESS 0
#define ERROR -1

typedef enum
{
    TOKEN_KEYWORD = 1,
    TOKEN_VARNAME,
    TOKEN_PUNCTUATION,
    TOKEN_QUOTATION,
    TOKEN_INT,
    TOKEN_ARITHMETIC,
    TOKEN_EQUALS,
    TOKEN_EOL,
    TOKEN_EOF,
    TOKEN_COMMA,
    TOKEN_ADDITION,
    TOKEN_SUBTRACTION,
    TOKEN_MULTIPLICATION,
    TOKEN_DIVISION,
    TOKEN_UNKNOWN,
} TokenType;

typedef enum
{
    KW_UNKNOWN = 0,
    KW_LET,
    KW_PRINT,
    KW_EOF,
} KeywordType;

typedef struct _TOKEN
{
    TokenType type;
    int token_num;
    char *lexeme;
} TOKEN, *PTOKEN;

const char *token_names[] = {
    "TOKEN_DUMMY", // For Enum starting at 1
    "TOKEN_KEYWORD",
    "TOKEN_VARNAME",
    "TOKEN_PUNCTUATION",
    "TOKEN_QUOTATION",
    "TOKEN_INT",
    "TOKEN_ARITHMETIC",
    "TOKEN_EQUALS",
    "TOKEN_EOL",
    "TOKEN_EOF",
    "TOKEN_COMMA",
    "TOKEN_ADDITION",
    "TOKEN_SUBTRACTION",
    "TOKEN_MULTIPLICATION",
    "TOKEN_DIVISION",
    "TOKEN_UNKNOWN",
};

const char *operation_names[] = {
    "TOKEN_ADDITION",
    "TOKEN_SUBTRACTION",
    "TOKEN_MULTIPLICATION",
    "TOKEN_DIVISION",
    "UNKNOWN_TYPE",
};

bool is_all_digits(char *word);
bool is_keyword(char *word);
bool is_math_symbol(char *word);
bool is_punctuation(char *word);
bool line_format_checker(PTOKEN arr_of_toks);
const char *get_token_type_name(TokenType type);
int get_Keyword_Enum(char *word);
int parse_file(char *filename);
long load_file(char *filename, char **buff);
PTOKEN tokenize_file(char *buff, long file_size);
TOKEN analyze_word(char *string);

// typedef enum
// {
//     TOKEN_ADDITION = 0,
//     TOKEN_SUBTRACTION,
//     TOKEN_MULTIPLICATION,
//     TOKEN_DIVISION,
//     TOKEN_OP_UNKNOWN,
// } OperationType;

// typedef struct _TOKEN
// {
//     TokenType type;
//     int token_num;
//     char *lexeme;
//     union
//     {
//         char varName;
//         int intValue;
//         OperationType operation;
//         KeywordType keyword;
//     } value;
// } TOKEN, *PTOKEN;

// typedef struct token
// {
//     TOKEN token;
//     struct token *next;
// } token_t;

// typedef struct node
// {
//     int line_num;
//     char *line;
//     token_t *tokens;
//     struct node *next;
//     struct node *prev;
// } node_t;

// char *load_file_contents(char *filename);
// const char *get_keyword_name(KeywordType kw);
// const char *get_token_operation_name(OperationType type);
// node_t *create_new_node(int line_num, char *line);
// node_t *parse_lines_to_nodes(char *file_contents);
// node_t *parse_node_word_by_word(node_t *head);
// TOKEN analyze_word(char *string, int line_num);
// TOKEN key_word_check(char *word, int line_num);
// void add_token_to_node(node_t *node, TOKEN token_info);
// void free_list(node_t *head);
// void insertAtEnd(node_t **head, node_t *new_node);
// void interpret_and_do_line(node_t *node);
// void parse_let(token_t *stream);
// void parse_print(token_t *stream);
// void tokenize_line(node_t *node);

// typedef struct
// {
//     KeywordType keyword;
//     void (*parse_function)(token_t *);
// } KeywordHandler;

// // Struct of function pointers for the Keyword handler struct. Assigns function to Keyword.
// KeywordHandler keyword_handlers[] = {
//     {KW_LET, parse_let},
//     {KW_PRINT, parse_print},
//     {KW_EOF, NULL} // should never be called
// };

// FROM basic_interpreter.c

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
