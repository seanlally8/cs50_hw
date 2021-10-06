#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

//task for next session: make code that will reject keys with non-alphabetic characters and characters that repeat.

//this program will provide a cypher text, given a key (entered as command-line argument) and plaintext.
int main(int argc, string argv[])
{
    string plaintext;

    //if user enters fewer or more than two command-line arguments, they will receive an error message: "Usage: ./substitution key".
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }

    //convert the entered key to all uppercase
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (islower(argv[1][i]))
        {
           argv[1][i]  = toupper(argv[1][i]);
        }
    }

    //Check to see if any characters are non-alphabetic.
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if ((argv[1][i] < 'A' || argv[1][i] > 'Z') && (argv[1][i] < 'a' || argv[1][i] > 'z'))
        {
            printf("Key must be entirely alphabetic and contain 26 non-repeating characters.\n");
            return 1;
            return 0;
        }
    }

    //check to see if any characters are repeating.
    for(int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        
    }

    //if the user enters fewer or more than 26 characters, they will receive an error message: "Key must contain 26 characters."
    if (strlen(argv[1]) < 26 || strlen(argv[1]) > 26)
    {
        printf("Key must be entirely alphabetic and contain 26 non-repeating characters.\n");
        return 1;
        return 0;
    }

    // if they enter exactly 26 characters, they will be prompted to enter plain text (to be converted to cyphertext).
    else
    {
        plaintext = get_string("plaintext: ");
        printf("cyphertext: ");
    }

    //the program will then output cypher text, maintaining upper/lower case stylization, and all spaces/punctuation.
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
         if (isupper(plaintext[i]) && isalpha(plaintext[i]))
         {
             int upper_cypher = plaintext[i] - 65;
             printf("%c", argv[1][upper_cypher]);
         }
         if (islower(plaintext[i]) && isalpha(plaintext[i]))
         {
             int lower_cypher = plaintext[i] - 97;
             printf("%c", argv[1][lower_cypher] + 32);
         }
         if ((plaintext[i] < 'A' || plaintext[i] > 'Z') && (plaintext[i] < 'a' || plaintext[i] > 'z'))
         {
             printf("%c", plaintext[i]);
         }
    }
    printf("\n");

}