// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

//Designate constant for James Aspnes' hash function
#define BASE (256)

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10007;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    return false;
}

// This hash function is taken from James Aspnes' (Professor of CS at Yale) notes
// at https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=%28CategoryAlgorithmNotes%29
unsigned int hash(const char *word)
{
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) word;

    h = 0;
    while(*us != '\0') {
        h = (h * BASE + *us) % N;
        us++;
    }

    return h;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Declare buffer into which we'll temporarily store each word
    char buffer[LENGTH + 1];

    // Open the dictionary so we can read out of it
    FILE *dict = fopen(dictionary, "r");

    //Check to make sure the file opens
    if (dict == NULL)
    {
        return false;
    }

    // Read each word out of the dictionary into the table, using the hash function defined above
    while ((fscanf(dict, "%s", buffer)) != EOF)
    {
        table[hash(buffer)] = malloc(sizeof(node));
        strcpy(table[hash(buffer)]->word, buffer);
    }

    //If we've reached the end of the file, we've succeeded, so we can return true
    if (fscanf(dict, "%s", buffer) == EOF)
    {
        return true;
    }

    //Otherwise, we've failed, so return false
    else
    {
        return false;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Note todo: perhaps write a simple count function and plug it into load,
    // so as we load the hash table, we can count the words
    if (load(dictionary) == 1)
    {

    }

    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    return false;
}
