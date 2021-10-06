#include <cs50.h>
#include <stdio.h>


int main(void)
{
    int n;
    do
    {
        n = get_int("Height: "); // prompt user for positive integer.
    }
    while (n < 1 || n > 8); // only allow integers between 1 and 8 inclusive.

// for n = 7, print 6 spaces and 1 hash on first row, 5 spaces and 2 hashes on second row etc.
    for (int i = 1; i <= n; i++)
    {
        for (int k = n - i; k > 0; k--)
        {
            printf(" ");
        }

        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}