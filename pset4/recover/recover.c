#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK 512

typedef uint8_t BYTE;

int filename = 0;

char filedigits[7];

int jpgcheck(BYTE *buffer);

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
        fclose(source);
        printf("Unable to open file");
        return 1;
    }


    BYTE *buffer = malloc(sizeof(BYTE) * BLOCK);
    if (buffer == NULL)
    {
        free(buffer);
        return 1;
    }

    while (fread(buffer, sizeof(BYTE), BLOCK, source) != EOF)
    {
        while(fread(buffer, sizeof(BYTE), BLOCK, source))
        {
            if (jpgcheck(buffer) == 1)
            {
                break;
            }
        }

        sprintf(filedigits, "%.3i.jpg", filename);
        FILE *newfile = fopen(filedigits, "w");
        filename++;

        while (fwrite(buffer, sizeof(BYTE), BLOCK, newfile))
        {
            fread(buffer, sizeof(BYTE), BLOCK, source);
            if (jpgcheck(buffer) == 1)
            {
                fclose(newfile);
                sprintf(filedigits, "%.3i.jpg", filename);
                newfile = fopen(filedigits, "w");
                filename++;
            }
        }
        fclose(newfile);
    }
    fclose(source);
    free(buffer);
    free(filedigits);
    return 0;
}

int jpgcheck(BYTE *buffer)
{
    if ((buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer [2] == 0xff) && (buffer[3] > 0xdf && buffer[3] < 0xf0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

