#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cs50.h>
#include <string.h>

#define BASE (256)
#define N 10007

unsigned long hash(const char *s);

int main(void)
{
    while(true)
    {
        char *word = get_string("Word: ");
        if (strcmp(word, "exit") == 0)
        {
            return 0;
        }

        printf("%lu\n", hash(word));
    }
}

unsigned long hash(const char *s)
{
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) s;

    h = 0;
    while(*us != '\0') {
        h = (h * BASE + *us) % N;
        us++;
    }

    return h;
}