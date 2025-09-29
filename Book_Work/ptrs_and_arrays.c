#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 256

int custom_strlen(char *str);
int custom_strcpy_array(char *src, char *dest);
int custom_strcpy_ptr(char *src, char *dest);

int main()
{
    char *str = "Hello World.";
    char buff[BUFFSIZE];
    custom_strlen(str);
    custom_strcpy_array(str, buff);
    custom_strcpy_ptr(str, buff);
    return 0;
}

int custom_strlen(char *str)
{
    // With ptr arithmetic
    char *p = str;
    size_t size = 0;
    while (*p != '\0')
    {
        p++;
    }
    size = p - str;
    printf("String size: %ld.\n", size);
    return 0;
}

int custom_strcpy_array(char *src, char *dest)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    printf("Str Cpy Array Dest: %s\n", dest);
    return 0;
}

int custom_strcpy_ptr(char *src, char *dest)
{
    char *ptr_dest = dest;
    while ((*ptr_dest = *src) != '\0')
    {
        src++;
        ptr_dest++;
    }
    printf("Str Cpy ptr Dest: %s\n", dest);
    return 0;
}