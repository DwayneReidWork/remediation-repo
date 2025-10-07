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
    TOKEN_COMMA,
    TOKEN_UNKNOWN,
} TokenType;

typedef enum
{
    TOKEN_ADDITION = 0,
    TOKEN_SUBTRACTION,
    TOKEN_MULTIPLICATION,
    TOKEN_DIVISION,
    TOKEN_OP_UNKNOWN,
} OperationType;

typedef enum
{
    KW_UNKNOWN = 0,
    KW_LET,
    KW_PRINT,
} KeywordType;

typedef struct _TOKEN
{
    TokenType type;
    int line_num;
    char *lexeme; // explain?
    union
    {
        char varName;
        int intValue;
        OperationType operation;
        KeywordType keyword;
    } value;
} TOKEN, *PTOKEN;

typedef struct token
{
    TOKEN token;
    struct token *next;
} token_t;

typedef struct node
{
    int line_num;
    char *line;
    token_t *tokens;
    struct node *next;
    struct node *prev;
} node_t;

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
    "TOKEN_COMMA",
    "TOKEN_UNKNOWN",
    "UNKNOWN_TYPE",
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
char *load_file_contents(char *filename);
const char *get_keyword_name(KeywordType kw);
const char *get_token_operation_name(OperationType type);
const char *get_token_type_name(TokenType type);
int get_Keyword_Enum(char *word);
node_t *create_new_node(int line_num, char *line);
node_t *parse_lines_to_nodes(char *file_contents);
node_t *parse_node_word_by_word(node_t *head);
TOKEN analyze_word(char *string, int line_num);
TOKEN key_word_check(char *word, int line_num);
void add_token_to_node(node_t *node, TOKEN token_info);
void free_list(node_t *head);
void insertAtEnd(node_t **head, node_t *new_node);
void interpret_and_do_line(node_t *node);
void tokenize_line(node_t *node);