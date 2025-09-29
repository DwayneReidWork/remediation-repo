/*
Pg 97 #5-1 (it looks like you basically did 5-2)
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BUFSIZE 100

int getch(void);
void ungetch(int c);
float getFloat(float *pn);

char buf[BUFSIZE];
int bufp = 0;

int main()
{
    float x;
    printf("Enter a number: ");
    if (getFloat(&x))
        printf("You entered: %.2f\n", x);
    else
        printf("Not a number!\n");
    return 0;
}

float getFloat(float *pn)
{
    int c, sign;
    float power;

    // skip leading whitespace
    while (isspace(c = getch()))
        ;

    // not a number, push back and return 0
    if (!isdigit(c) && c != EOF && c != '+' && c != '-')
    {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-')
    {
        int next = getch();
        if (!isdigit(next) && next != '.')
        {
            // not a number after +/-, push both back
            ungetch(next);
            ungetch(c);
            return 0;
        }
        c = next;
    }

    // integer part
    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10.0 * *pn + (c - '0');

    // fractional part (for float stuff)
    if (c == '.')
    {
        c = getch();
        for (power = 1.0; isdigit(c); c = getch())
        {
            *pn = 10.0 * *pn + (c - '0');
            power *= 10.0;
        }
        *pn /= power;
    }

    *pn *= sign;

    if (c != EOF)
        ungetch(c);
    return c;
}

// Get single char from buffer
int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

// Push char back into buffer
void ungetch(int c)
{
    if (bufp >= BUFSIZE)
    {
        printf("ungetch: too many charachters\n");
    }
    else
    {
        buf[bufp++] = c;
    }
}