#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
    char buffer[7];
    int filename = 0;
    sprintf(buffer, "%.3i.jpg", filename);

    for (int i = 0; i < 11; i++)
    {
        printf("%s\n", buffer);
        filename++;
    }
}