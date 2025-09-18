#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_ROWS 10
#define GRID_COLS 10

typedef struct
{
    int id;
    char *name;
    char *category;
    int quantity;
    float price;
} Item;

/*
Item struct
Doubly linked list struct

Necessary funcs
int main(int argc, char* argv[])
{
on Launch check for inventory.txt and load (create if not)
in loop prompt user for input to add/search/update/delete/list
on exit of loop save file
}

open/load file(filename?, *struct)
{
Should I pass the filename as a param or just define it as a global constant
fread struct from file field by field
malloc for name and category
frees
}


save file(filename?, *struct)
{
fwrite struct to file Field by field.
malloc for name and category
frees
}

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
