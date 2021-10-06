// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    printf("%f\n", factor);



    // TODO: Copy header from input file to output file
    uint8_t* buffer = malloc(sizeof(uint8_t) * HEADER_SIZE);
    fread(buffer, sizeof(uint8_t), HEADER_SIZE, input);

    fwrite(buffer, sizeof(uint8_t), HEADER_SIZE, output);


    // TODO: Read samples from input file and write updated data to output file
    int16_t buffer2;
    while (fread(&buffer2, sizeof(uint16_t), 1 ,input))
    {

            buffer2 = buffer2 * factor;
            //printf("%u\n", buffer2);

        fwrite(&buffer2, sizeof(uint16_t), 1, output);
    }


    // Close files
    free(buffer);
    fclose(input);
    fclose(output);
}
