#define ERROR -1

typedef struct node
{
    int line_num;
    char *line;
    struct node *next;
    struct node *prev;
} node_t;

char *load_file_contents(char *filename);
node_t *create_new_node(int line_num, char *line);
node_t *parse_lines_to_nodes(char *file_contents);
node_t *parse_node_for_cmds(node_t *head);
void free_list(node_t *head);
void insertAtEnd(node_t **head, node_t *new_node);