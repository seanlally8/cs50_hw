#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    FILE *file = fopen("phonebook.csv", "a"); //FILE is a C datatype, two arguments, file to open, and "a" for append
    if (file == NULL)
    {
        return 1; //just make sure it's been allocated
    }

    char *name = get_string("Name: ");
    char *number = get_string("Number: ");

    fprintf(file, "%s,%s\n", name, number);

    fclose(file);

}