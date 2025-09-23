/*
Pg 189 #8-6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *custom_calloc(size_t num_elements, size_t size);

int main()
{
    int num_elements = 50;
    int size = sizeof(int);
    custom_calloc(num_elements, size);
    return 0;
}

/// @brief Custom calloc
/// @param num_elements how many elements you want
/// @param size size of each element
/// @return pointer to memory
int *custom_calloc(size_t num_elements, size_t size)
{
    size_t total_size = num_elements * size;
    int *mem_block = malloc(total_size);
    if (mem_block == NULL)
    {
        return NULL;
    }

    memset(mem_block, 0, total_size);

    return mem_block;
}
