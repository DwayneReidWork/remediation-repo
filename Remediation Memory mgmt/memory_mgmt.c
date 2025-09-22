#include <errno.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 256
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
    Item item_data;
    struct Node *next;
    struct Node *prev;
} Node;

// Funcs
char *read_string(FILE *fp);
int add_item(Node **head);
int exit_cleanup(Item *item_ptr);
int load(Node **head);
int parse_num_choice();
int prompt_user_selection(bool keep_looping, Node *head);
int remove_item(Node *head, Item *item_to_add);
int save(Node *head);
int search_item(Node *head, int id);
int update_item(Node *head, int id);
int validate_name_or_category(char *str);
float parse_float();

// Node Funcs
int getLastNode(Node *head);
Node *createNode(Item data);
void deleteAtPosition(Node **head, int position);
void freeNode(Node *temp);
void freeList(Node **head);
void insertAtEnd(Node **head, Item data);
void printListForward(Node *head);

int main(int argc, char *argv[])
{
    Item *item_ptr = malloc(sizeof(Item));

    if (NULL == item_ptr)
    {
        printf("Malloc failed\n");
        return ERROR;
    }

    Node *head = NULL;

    load(&head);

    // in loop prompt user for input to add/search/update/delete/list
    // on exit of loop save file

    bool keepLooping = true;
    while (keepLooping)
    {
        keepLooping = prompt_user_selection(keepLooping, head);
    }
    // // dummy data
    // item_ptr->id = 1;
    // // Use strdup so that malloc can free somehing and not break cleanup
    // item_ptr->category = strdup("Vegetable");
    // item_ptr->name = strdup("Carrot");
    // item_ptr->quantity = 11;
    // item_ptr->price = 2.99;
    // insertAtEnd(&head, *item_ptr);
    // printListForward(head);

    printf("Saving. . .\n");
    save(head);
    printf("Memory Cleanup.\n");
    exit_cleanup(item_ptr);
    freeList(&head);
    printf("Exiting.\n");
    return 0;
}

int prompt_user_selection(bool keepLooping, Node *head)
{
    int selection;
    printf("Please make a selection(1-6): \n1. Add Item\n2. Search for item\n3. Update Item\n4. delete Item\n5. List items\n6. Exit\n");
    selection = parse_num_choice();

    switch (selection)
    {
    case 1:
        add_item(&head);
        break;
    case 2:
        printf("Please input the ID number of the item you wish to search.\n");
        selection = parse_num_choice();
        search_item(head, selection);
        break;
    case 3:
        printf("Please input the ID number of the item you wish to update.\n");
        selection = parse_num_choice();
        update_item(head, selection);
        break;
    case 4:
        printf("Please input the ID number of the item you wish to delete.\n");
        selection = parse_num_choice();
        deleteAtPosition(&head, selection);
        break;
    case 5:
        printListForward(head);
        break;
    case 6:
        keepLooping = 0;
        break;
    default:
        printf("Please select a number 1 through 6.\n");
        break;
    }
    return keepLooping;
}

int load(Node **head)
{
    FILE *fp = fopen(FILEPATH, "rb");
    if (!fp)
    {
        printf("No file to load.\n");
        return ERROR;
    }

    while (!feof(fp))
    {
        Item item;
        int read_id = fread(&item.id, sizeof(int), 1, fp);
        if (read_id != 1)
            // stop at EOF or read error
            break;

        item.name = read_string(fp);
        if (!item.name)
            // failed read
            break;

        item.category = read_string(fp);
        if (!item.category)
            break;

        fread(&item.quantity, sizeof(int), 1, fp);
        fread(&item.price, sizeof(float), 1, fp);

        insertAtEnd(head, item);
        free(item.category);
        free(item.name);
    }

    fclose(fp);
    return 0;
}

int save(Node *head)
{
    if (!head)
    {
        printf("Nothing to save.\n");
        return ERROR;
    }

    FILE *fp = fopen(FILEPATH, "wb");
    if (!fp)
    {
        printf("Error opening file to save: %s\n", strerror(errno));
        return ERROR;
    }

    Node *current = head;
    while (current != NULL)
    {
        fwrite(&current->item_data.id, sizeof(int), 1, fp);
        fwrite(current->item_data.name, strlen(current->item_data.name) + 1, 1, fp);
        fwrite(current->item_data.category, strlen(current->item_data.category) + 1, 1, fp);
        fwrite(&current->item_data.quantity, sizeof(int), 1, fp);
        fwrite(&current->item_data.price, sizeof(float), 1, fp);

        current = current->next;
    }

    fclose(fp);
    return 0;
}

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

int exit_cleanup(Item *item_ptr)
{
    if (!item_ptr)
    {
        return 0;
    }
    free(item_ptr->name);
    free(item_ptr->category);
    free(item_ptr);
    return 0;
}

Node *createNode(Item data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode->item_data.id = data.id;
    newNode->item_data.name = strdup(data.name);
    newNode->item_data.category = strdup(data.category);
    newNode->item_data.quantity = data.quantity;
    newNode->item_data.price = data.price;

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

void deleteAtPosition(Node **head, int position)
{
    if (!head || !*head)
    {
        printf("The list is already empty.\n");
        return;
    }

    Node *temp = *head;

    // If index is 1, remove head
    if (position == 1)
    {
        *head = temp->next;
        if (*head)
        {
            (*head)->prev = NULL;
        }
        freeNode(temp);
        return;
    }

    // Traverse to the node at the given position
    for (int i = 1; temp != NULL && i < position; i++)
    {
        temp = temp->next;
    }

    // If index is out of range
    if (temp == NULL)
    {
        printf("Position is greater than the number of nodes.\n");
        return;
    }

    // Unlink the node
    if (temp->prev)
    {
        temp->prev->next = temp->next;
    }
    if (temp->next)
    {
        temp->next->prev = temp->prev;
    }

    freeNode(temp);
}

void printListForward(Node *head)
{
    if (NULL == head)
    {
        printf("List empty.\n");
        return;
    }

    Node *temp = head;

    printf("\nForward List:\n");
    while (temp != NULL)
    {
        printf("ID: %d\nName: %s\nCategory: %s\nQuantity: %d\nPrice: $%.2f\n\n", temp->item_data.id,
               temp->item_data.name,
               temp->item_data.category,
               temp->item_data.quantity,
               temp->item_data.price);
        temp = temp->next;
    }
    printf("\n");
}

int getLastNode(Node *head)
{
    if (NULL == head)
    {
        return 0;
    }
    else
    {
        struct Node *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        return current->item_data.id;
    }
}

void freeNode(Node *node)
{
    if (!node)
        return;
    free(node->item_data.name);
    free(node->item_data.category);
    free(node);
}

int add_item(Node **head)
{
    char *category;
    char *name;
    char tmp_buff[BUFF_SIZE];
    float price = 0.0;
    int c;
    int quantity = 0;
    Item item_to_add;
    size_t len = 0;

    // Clear buffer
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    int end = getLastNode(*head);
    item_to_add.id = end + 1;

    // Get Item name
    printf("Please enter the name of the item:\n");

    if (NULL == fgets(tmp_buff, BUFF_SIZE, stdin))
    {
        printf("Error reading name.\n");
        return ERROR;
    }
    // Null Terminate buffer
    tmp_buff[strcspn(tmp_buff, "\n")] = '\0';
    len = strnlen(tmp_buff, BUFF_SIZE - 1);

    // Calloc struct based on buffer size
    name = calloc(len, sizeof(char));

    // copy data because calloc zeroes out buff
    memcpy(name, tmp_buff, len);

    // validate input
    if (1 != validate_name_or_category(name))
    {
        printf("Name {%s} failed parse check.\n", name);
        return ERROR;
    }

    // copy it to struct
    item_to_add.name = name;

    // Get Category
    printf("Please enter the category for {%s}.\n", name);

    if (NULL == fgets(tmp_buff, BUFF_SIZE, stdin))
    {
        printf("Error reading category.\n");
        return ERROR;
    }
    // Null Terminate buffer
    tmp_buff[strcspn(tmp_buff, "\n")] = '\0';
    len = strnlen(tmp_buff, BUFF_SIZE - 1);

    // Malloc struct based on buffer size
    category = calloc(len, sizeof(char));

    // copy data
    memcpy(category, tmp_buff, len);

    if (1 != validate_name_or_category(category))
    {
        printf("Category {%s} failed parse check.\n", category);
    }

    item_to_add.category = category;

    // Get Quantity
    printf("Please enter the quantity for {%s}.\n", name);
    quantity = parse_num_choice();
    {
        item_to_add.quantity = quantity;
    }

    printf("Please enter the price for {%s}.\n", name);
    price = parse_float();
    {
        item_to_add.price = price;
    }

    insertAtEnd(head, item_to_add);
    return 0;
}

int update_item(Node *head, int id)
{
    if (NULL == head || 0 == id)
    {
        printf("Invalid head ptr or ID recv'd.\n");
        return ERROR;
    }

    char tmp_buff[BUFF_SIZE];
    Node *current = head;

    while (current != NULL)
    {
        int quantity = 0;
        float price = 0.0;

        if (current->item_data.id == id)
        {
            // Clear buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }

            char *name;
            char *category;

            // Get Item name
            printf("Please enter the name of the item:\n");

            if (NULL == fgets(tmp_buff, BUFF_SIZE, stdin))
            {
                printf("Error reading name.\n");
                return ERROR;
            }
            // Null Terminate buffer
            tmp_buff[strcspn(tmp_buff, "\n")] = '\0';
            size_t len = strnlen(tmp_buff, BUFF_SIZE - 1);

            // Calloc struct based on buffer size
            name = calloc(len, sizeof(char));

            // copy data because calloc zeroes out buff
            memcpy(name, tmp_buff, len);

            // validate input
            if (1 != validate_name_or_category(name))
            {
                printf("Name {%s} failed parse check.\n", name);
                free(name);
                return ERROR;
            }

            // copy it to struct
            current->item_data.name = name;

            // Get Category
            printf("Please enter the category for {%s}.\n", name);

            if (NULL == fgets(tmp_buff, BUFF_SIZE, stdin))
            {
                printf("Error reading category.\n");
                free(category);
                return -ERROR;
            }
            tmp_buff[strcspn(tmp_buff, "\n")] = '\0';

            // Malloc struct based on buffer size
            category = calloc(len, sizeof(char));

            // copy data
            memcpy(category, tmp_buff, len);
            if (1 != validate_name_or_category(category))
            {
                printf("Category {%s} failed parse check.\n", category);
                free(category);
                return -ERROR;
            }

            current->item_data.category = category;

            // Get Quantity
            printf("Please enter the quantity for {%s}.\n", name);
            quantity = parse_num_choice();
            {
                current->item_data.quantity = quantity;
            }

            printf("Please enter the price for {%s}.\n", name);
            price = parse_float();
            {
                current->item_data.price = price;
            }
            return 0;
        }
        current = current->next;
    }

    printf("Id not found\n");
    return ERROR;
}

int remove_item(Node *head, Item *item_to_add)
{
    int selection;
    printf("Please enter the ID of the item you wish to remove.\n");
    if (1 != scanf("%d", &selection))
    {
        printf("Invalid selection. Must be numerical.\n");
        // clear buffer so no infinity loop
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; // On a new line because its technically after an empty curly brace
    }
    else
    {
        deleteAtPosition(&head, selection);
    }
    return 0;
}

/// @brief Parse string confirming only letters nums and spaces
/// @param str string to analyze
/// @return 1 on match -1 on fail
int validate_name_or_category(char *str)
{
    if (NULL == str)
    {
        printf("String is null.\n");
        return ERROR;
    }

    regex_t regex;
    const char *pattern = "^[A-Za-z0-9 ]+$";
    int ret;

    if (regcomp(&regex, pattern, REG_EXTENDED))
    {
        printf("Could not compile regex\n");
        return ERROR;
    }

    ret = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);

    if (ret == 0)
    {
        return 1; // match → valid
    }
    else
    {
        return ERROR; // no match → invalid
    }
}

int search_item(Node *head, int id)
{
    if (NULL == head || 0 == id)
    {
        printf("Invalid head ptr or ID recv'd.\n");
        return ERROR;
    }

    Node *current = head;

    while (current != NULL)
    {
        if (current->item_data.id == id)
        {
            printf("ID: %d\nName: %s\nCategory: %s\nQuantity: %d\nPrice: %.2f\n",
                   current->item_data.id,
                   current->item_data.name,
                   current->item_data.category,
                   current->item_data.quantity,
                   current->item_data.price);
            return 0;
        }
        current = current->next;
    }

    printf("Id not found\n");
    return ERROR;
}

int parse_num_choice()
{
    int selection;
    if (1 != scanf("%d", &selection))
    {
        printf("Invalid selection. Must be numerical\n");
        // clear buffer so no infinity loop
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; // On a new line because its technically after an empty curly brace
    }
    return selection;
}

float parse_float()
{
    float selection;
    if (1 != scanf("%f", &selection))
    {
        printf("Invalid selection. Must be numerical (float)\n");
        // clear buffer so no infinity loop
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; // On a new line because its technically after an empty curly brace
        return 0.0f;
    }

    return selection;
}

void freeList(Node **head)
{
    Node *current = *head;
    Node *next;
    while (current)
    {
        next = current->next;
        freeNode(current);
        current = next;
    }
    *head = NULL;
}