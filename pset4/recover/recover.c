#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Define block size
#define BLOCK 512

//make datatype more readable
typedef uint8_t BYTE;

//initialize variable for .jpg filenames (000.jpg, 001.jpg etc)
int filename = 0;

//set array size for sprintf call
char filedigits[7];

//Prototype function for checking jpeg signatures
int jpgcheck(BYTE *buffer);

int main(int argc, char *argv[])
{
    //make sure user has entered the correct number of commands/arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    //open the file entered by user (in this case, card.raw)
    FILE *source = fopen(argv[1], "r");
    if (source == NULL)
    {
        printf("Unable to open file");
        return 1;
    }

    //Set aside memory for buffer to read into and write out of
    BYTE *buffer = malloc(sizeof(BYTE) * BLOCK);
    if (buffer == NULL)
    {
        free(buffer);
        return 1;
    }

    //Check for the initial jpeg signatures
    while (fread(buffer, sizeof(BYTE), BLOCK, source))
    {
        if (jpgcheck(buffer) == 1)
        {
            break;
        }
    }

    //set initial output filename (000.jpg)
    sprintf(filedigits, "%.3i.jpg", filename);

    //open a new file and name it using the array designated by sprintf
    FILE *newfile = fopen(filedigits, "w");

    //Check to make sure file opens
    if (newfile == NULL)
    {
        fclose(newfile);
        return 1;
    }

    //Increment variable used in sprintf function to name output files (so the next one is 001.jpg)
    filename++;

    //since the last i/o function was fread, we write out of the allocated buffer into the newly opened file
    fwrite(buffer, sizeof(BYTE), BLOCK, newfile);
    
    //Continue to read from the source file into the newly opened file,
    //and when a new jpeg signature is found, close that file and open another one to write into.
    while (fread(buffer, sizeof(BYTE), BLOCK, source))
    {
        if (jpgcheck(buffer) == 1)
        {
            fclose(newfile);
            sprintf(filedigits, "%.3i.jpg", filename);
            newfile = fopen(filedigits, "w");
            if (newfile == NULL)
            {
                fclose(newfile);
                return 1;
            }
            filename++;
        }

        fwrite(buffer, sizeof(BYTE), BLOCK, newfile);

    }

    //close the output jpg file
    fclose(newfile);
    
    //close the source file (entered by the user)
    fclose(source);
    
    //free the memory allocated for the buffer
    free(buffer);
    
    //Exit program with no errors
    return 0;
}

//Function to check for jpeg signatures
int jpgcheck(BYTE *buffer)
{
    //if the signature is found return 1, otherwise return a number that isn't 1 (I've chosen 2 arbitrarily)
    if ((buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer [2] == 0xff) && (buffer[3] > 0xdf && buffer[3] < 0xf0))
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

