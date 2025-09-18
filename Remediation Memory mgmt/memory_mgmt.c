#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define FILEPATH "inventory.txt"

// Item struct
// Doubly linked list struct
typedef struct
{
    int id;
    char *name;
    char *category;
    int quantity;
    float price;
} Item;

typedef struct Node
{
    Item data;
    struct Node *next;
    struct Node *prev;
} Node;

// Funct
char *read_string(FILE *fp);
int exit_cleanup(Item *item_ptr);
int open_load(Item *item_ptr);
int prompt_user();
int save(Item *item_ptr);

// Node Funcs
Node *createNode(Item data);
void insertAtEnd(Node **head, Item data);
void deleteAtBeginning(Node **head);
void deleteAtPosition(Node **head, int position);
void deleteAtEnd(Node **head);
void printListForward(Node *head);

int main(int argc, char *argv[])
{
    Item *item_ptr = malloc(sizeof(Item));
    if (NULL == item_ptr)
    {
        printf("Malloc failed\n");
        return -1;
    }

    open_load(item_ptr);

    // in loop prompt user for input to add/search/update/delete/list
    // on exit of loop save file
    while (1)
    {
        prompt_user();
    }
    save(item_ptr);

    printf("Memory Cleanup.\n");
    exit_cleanup(item_ptr);
    printf("Exiting.\n");
    return 0;
}

int prompt_user()
{
    int selection;
    printf("Please make a selection(1-6): \n1. Add Item\n2. Search for item\n3. Update Item\n4. delete Item\n5. List items\n6. Exit\n");
    scanf("%d", &selection);

    switch (selection)
    {
    case 1:
        printf("Case %d recieved\n", selection);
        break;
    case 2:
        printf("Case %d recieved\n", selection);
        break;
    case 3:
        printf("Case %d recieved\n", selection);
        break;
    case 4:
        printf("Case %d recieved\n", selection);
        break;
    case 5:
        printf("Case %d recieved\n", selection);
        break;
    case 6:
        printf("Case %d recieved\n", selection);
        break;
    default:
        printf("Please select a number 1 through 6.\n");
        break;
    }
}

int open_load(Item *item_ptr)
{
    if (NULL == item_ptr)
    {
        printf("Null Pointer passed in.\n");
        return ERROR;
    }

    memset(item_ptr, 0, sizeof(Item));
    FILE *fp = fopen(FILEPATH, "rb");

    if (NULL == fp)
    {
        printf("Error opening file: %s\n", strerror(errno));
        return ERROR;
    }

    // fread struct from file field by field
    fread(&item_ptr->id, sizeof(item_ptr->id), 1, fp);
    // Read string mallocs and deals with ptr
    item_ptr->name = read_string(fp);
    item_ptr->category = read_string(fp);
    fread(&item_ptr->quantity, sizeof(item_ptr->quantity), 1, fp);
    fread(&item_ptr->price, sizeof(item_ptr->price), 1, fp);

    // frees (In main)
    free(read_string(fp));
    printf("Load Successful.\n");

    return 0;
}
int save_file(Item *item_ptr)
{
    if (NULL == item_ptr)
    {
        printf("Null Pointer passed in.\n");
        return ERROR;
    }

    FILE *fp = fopen(FILEPATH, "wb");

    if (NULL == fp)
    {
        printf("Error opening file to save : %s\n", strerror(errno));
        return ERROR;
    }
    fwrite(&item_ptr->id, sizeof(item_ptr->id), 1, fp);
    fwrite(item_ptr->name, strlen(item_ptr->name), 1, fp);
    fwrite(&item_ptr->category, strlen(item_ptr->category), 1, fp);
    // TODO
    fwrite(&item_ptr->quantity, sizeof(item_ptr->quantity), 1, fp);
    fwrite(&item_ptr->price, sizeof(item_ptr->price), 1, fp);
}

/*
delete item(Node *head, id)
{
search through LL for id and remove updating prev->next and next->last
}

update item(Node *head, id)
{
regex check for valid quantity/price/name
search through LL for id and edit node->item->price
}

add item(Node *head, id)
{
regex check for valid quantity/price/name
add item to end
}

list item(*head)
{
iterate through list until null printing each item
}

search item(*head)
{
secondary prompt for name - category
(or name by default and category with flag) [argparse]

}
*/

char *read_string(FILE *fp)
{
    int size = 0;
    int status = 0;
    long start_pos = 0;
    char *output = NULL;
    int ch;

    // Save current file position
    start_pos = ftell(fp);
    if (start_pos == ERROR)
    {
        perror("Cannot determine file position");
        return NULL;
    }

    // First pass: count characters until null terminator
    while ((ch = fgetc(fp)) != EOF && ch != '\0')
    {
        size++;
    }

    if (ch == EOF)
    {
        fprintf(stderr, "Unexpected EOF\n");
        return NULL;
    }

    // Allocate memory (+1 for null terminator)
    output = malloc(size + 1);
    if (output == NULL)
    {
        perror("Failed to allocate");
        return NULL;
    }

    // Rewind to start of string
    status = fseek(fp, start_pos, SEEK_SET);
    if (status != 0)
    {
        perror("Cannot set file position");
        free(output);
        return NULL;
    }

    // Read string into memory
    status = fread(output, sizeof(char), size, fp);
    if (status != size)
    {
        perror("Failed to read string");
        free(output);
        return NULL;
    }
    output[size] = '\0'; // null-terminate

    // Move file pointer past the null terminator
    fgetc(fp);

    return output;
}

int exit_free(Item *item_ptr)
{
    free(item_ptr);
    return 0;
}

Node *createNode(Item data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void insertAtEnd(Node **head, Item data)
{
    // creating new node
    Node *newNode = createNode(data);

    // check if DLL is empty
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    Node *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
}

void deleteAtBeginning(Node **head)
{
    // checking if the DLL is empty
    if (*head == NULL)
    {
        printf("The list is already empty.\n");
        return;
    }
    Node *temp = *head;
    *head = (*head)->next;
    if (*head != NULL)
    {
        (*head)->prev = NULL;
    }
    free(temp);
}

void deleteAtPosition(Node **head, int position)
{
    if (*head == NULL)
    {
        printf("The list is already empty.\n");
        return;
    }
    Node *temp = *head;
    if (position == 1)
    {
        deleteAtBeginning(head);
        return;
    }
    for (int i = 1; temp != NULL && i < position; i++)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("Position is greater than the number of "
               "nodes.\n");
        return;
    }
    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }
    if (temp->prev != NULL)
    {
        temp->prev->next = temp->next;
    }
    free(temp);
}

void deleteAtEnd(Node **head)
{
    // checking if DLL is empty
    if (*head == NULL)
    {
        printf("The list is already empty.\n");
        return;
    }

    Node *temp = *head;
    if (temp->next == NULL)
    {
        *head = NULL;
        free(temp);
        return;
    }
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->prev->next = NULL;
    free(temp);
}

void printListForward(Node *head)
{
    Node *temp = head;
    printf("Forward List: ");
    while (temp != NULL)
    {
        printf("ID: %d\nName: %s\nCategory: %s\nQuantity: %d\nPrice: %.2f\n", temp->data.id,
               temp->data.name,
               temp->data.category,
               temp->data.quantity,
               temp->data.price);
        temp = temp->next;
    }
    printf("\n");
}