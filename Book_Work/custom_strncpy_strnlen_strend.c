/*
Pg 107 #4 & #5
*/

#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 256

int custom_strend(char *haystack, char *needle);
int custom_strlen(char *str);
int custom_strncmp(char *str1, char *str2, int limit);
int custom_strncpy_array(char *src, char *dest, int limit);
int custom_strncpy_ptr(char *src, char *dest, int limit);

int main()
{
    char *str = "Hello World!";
    char *str2 = "Goodbye World!";
    char *str3 = "World!";
    int limit = 5;
    int status = 0;
    char buff[BUFFSIZE];

    custom_strncpy_array(str, buff, limit);
    custom_strncpy_ptr(str, buff, limit);

    status = custom_strend(str, str2);
    printf("Custom strend output for {%s} and {%s} is {%d}\n", str, str2, status);

    status = custom_strend(str2, str3);
    printf("Custom strend output for {%s} and {%s} is {%d}\n", str2, str3, status);

    status = custom_strncmp(str, str2, limit);
    printf("Custom strncmp Output:\nComparing first %d chars of strings\n%s\n%s\nResult: %d\n", limit, str, str2, status);

    return 0;
}

/// @brief Returns length of a string using ptr arithmetic
/// @param str any string
/// @return size_t size
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
    return size;
}

/// @brief copies string from source to dest up to n chars
/// @param src source
/// @param dest destination
/// @param limit n
/// @return 0 on success (prints dest)
int custom_strncpy_array(char *src, char *dest, int limit)
{
    int i = 0;
    while (src[i] != '\0' && i < limit)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    printf("Str Cpy Array Dest: %s\n", dest);
    return 0;
}

int custom_strncpy_ptr(char *src, char *dest, int limit)
{
    // Startting at 1 because no 0 indexing?
    int i = 1;
    char *ptr_dest = dest;
    while ((*ptr_dest = *src) != '\0' && i < limit)
    {
        src++;
        ptr_dest++;
        i++;
    }
    printf("Ptr Strcpy\nStr Cpy Array Dest: %s\n", dest);
    return 0;
}

int custom_strncmp(char *str1, char *str2, int limit)
{
    for (int i = 0; i < limit; i++)
    {
        if (str1[i] == '\0' && str2[i] == '\0')
            return 0;
        if (str1[i] == '\0')
            return -1;
        if (str2[i] == '\0')
            return 1;

        if (str1[i] < str2[i])
            return -1;
        if (str1[i] > str2[i])
            return 1;
    }
    return 0;
}

int custom_strend(char *haystack, char *needle)
{
    int len_str1, len_str2;
    len_str1 = custom_strlen(haystack);
    len_str2 = custom_strlen(needle);

    if (len_str2 > len_str1)
    {
        printf("{%s} is too long to be at the end of {%s}.\n", needle, haystack);
        return -1;
    }

    // move pointer to start of comparison
    haystack += (len_str1 - len_str2);

    while (*haystack != '\0' && *needle != '\0' && *haystack == *needle)
    {
        haystack++;
        needle++;
    }

    if (*haystack == '\0')
    {
        // success
        return 0;
    }
    else
    {
        // fail
        return -1;
    }
}