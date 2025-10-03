#define ERROR -1
// #define MAX_VAR_NAME 64

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
    TOKEN_UNKNOWN,
} TokenType;

typedef enum
{
    TOKEN_ADDITION = 1,
    TOKEN_SUBTRACTION,
    TOKEN_MULTIPLICATION,
    TOKEN_DIVISION
} OperationType;

typedef struct _TOKEN
{
    TokenType type;
    union
    {
        char varName;
        int intValue;
        OperationType operation;
    } value;
} TOKEN, *PTOKEN;

typedef struct node
{
    int line_num;
    char *line;
    struct node *next;
    struct node *prev;
} node_t;

const char *token_names[] = {
    "TOKEN_DUMMY", // FOr Enum starting at 1
    "TOKEN_KEYWORD",
    "TOKEN_VARNAME",
    "TOKEN_PUNCTUATION",
    "TOKEN_QUOTATION",
    "TOKEN_INT",
    "TOKEN_ARITHMETIC",
    "TOKEN_EQUALS",
    "TOKEN_EOL",
    "TOKEN_UNKNOWN",
    "UNKNOWN_TYPE",
};

bool is_all_digits(char *word);
bool is_keyword(char *word);
bool is_math_symbol(char *word);
bool is_punctuation(char *word);
char *load_file_contents(char *filename);
const char *get_token_type_name(TokenType type);
int analyze_word(char *string);
node_t *create_new_node(int line_num, char *line);
node_t *parse_lines_to_nodes(char *file_contents);
node_t *parse_node_word_by_word(node_t *head);
void free_list(node_t *head);
void insertAtEnd(node_t **head, node_t *new_node);
void tokenize_line(char *line);