/*
Pg: 110 #5-7. Ignore the "how much faster" bit unless you want to do that.
*/

/*
When running program you can enter as many single chars each seperated by enter
When complete press ctrl+d to get a count of total lines entered
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1000
#define MAXLINES 5000

int custom_getline(char s[], int limit);
int custom_readlines(char *lineptr[], int max_lines);
int custom_readlines_updated(char *lineptr[], int max_lines);
void write_lines(char *lineptr[], int nlines);

char *lineptr[MAXLINES];

int main()
{
    int nlines = custom_readlines(lineptr, MAXLINES);
    if (nlines < 0)
    {
        printf("Error: too many lines or out of memory.\n");
        return 1;
    }

    printf("\nRead %d lines:\n", nlines);
    for (int i = 0; i < nlines; i++)
    {
        printf("[%d] %s\n", i, lineptr[i]);
    }

    // Caller? free since malloc is done inside the other func?
    for (int i = 0; i < nlines; i++)
    {
        free(lineptr[i]);
    }
    return 0;
}

int custom_readlines(char *lineptr[], int max_lines)
{
    int len;
    int nlines = 0;
    char *p, line[MAXLEN];

    while ((len = custom_getline(line, MAXLEN)) > 0)
    {
        if (nlines >= max_lines || (p = malloc(len + sizeof(char))) == NULL)
        {
            return -1;
        }
        else
        {
            line[len - 1] = '\0';
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    lineptr[nlines] = NULL; // mark end for convenience
    return nlines;
}

void write_lines(char *lineptr[], int nlines)
{
    for (int i = 0; i < nlines; i++)
    {
        printf("%s\n,", lineptr[i]);
    }
}

int custom_readlines_updated(char *lineptr[], int max_lines)
{
    return 0;
}

/// @brief Waits for user to enter a char from cmd line  and press enter
/// @param s buffer
/// @param limit n
/// @return length
int custom_getline(char s[], int limit)
{
    int c, i;
    for (i = 0; i < limit - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
    {
        s[i] = c;
    }
    if (c == '\n')
    {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

/*
112 5-8

189 8-6
*/