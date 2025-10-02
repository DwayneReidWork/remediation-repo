#define ERROR -1

typedef enum
{
    TOKEN_PRINT = 1,
    TOKEN_LET,
    TOKEN_GO_TO,
    TOKEN_IF,
    TOKEN_GOSUB,
    TOKEN_RETURN,
    TOKEN_VARNAME,
    TOKEN_EQUALS,
    TOKEN_INT,
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

char *load_file_contents(char *filename);
int analyze_word(char *string);
node_t *create_new_node(int line_num, char *line);
node_t *parse_lines_to_nodes(char *file_contents);
node_t *parse_node_word_by_word(node_t *head);
void free_list(node_t *head);
void insertAtEnd(node_t **head, node_t *new_node);
void tokenize_line(char *line);