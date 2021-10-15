#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK 512

typedef uint8_t BYTE;

int filename = 0;

char filedigits[7];


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    if (source == NULL)
    {
        printf("Unable to open file");
        return 1;
    }


    BYTE *buffer = malloc(sizeof(BYTE) * BLOCK);
    sprintf(filedigits, "%.3i.jpg", filename);

    while (fread(buffer, sizeof(BYTE), BLOCK, source))
    {
        if ((buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer [2] == 0xff) && (buffer[3] > 0xdf && buffer[3] < 0xf0))
        {
            FILE *newfile = fopen(filedigits, "w");
            while (fread(buffer, sizeof(BYTE), BLOCK, source))
            {
                fwrite(buffer, sizeof(BYTE), BLOCK, newfile);
            }
            fclose(newfile);
        }
        filename++;
    }
}