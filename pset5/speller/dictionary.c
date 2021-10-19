// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>

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

// Prototype free_l function
bool free_l(node *head);

// Assign bool variable for determining whether load succeeded
bool loaded = false;

// Number of buckets in hash table
const unsigned int N = 10007;

// Designate global counter for size function
unsigned int counter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Assign return value of hash function to a variable for convenience
    unsigned int index = hash(word);

    // Generate search pointer initialized to the first element in linked list at hash/index
    node *search = table[index];

    //Search list for word by incrementing across the elements
    while (search != NULL)
    {
        int i = strcasecmp(word, search->word);
        if (i == 0)
        {
            return true;
        }

        else if (i != 0)
        {
            search = search->next;
        }
    }

    // If we've made it through the previous loop without returning true, it means we haven't found the word and we can return false
    return false;
}

/*A case-insensitive implementation of the djb2 hash function.

 can be found on github: https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c

 Adapted by Neel Mehta from
 http://stackoverflow.com/questions/2571683/djb2-hash-function. */
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + tolower(*ptr);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Declare buffer into which we'll temporarily store each word
    char buffer[LENGTH + 1];

    // Initialize table[] to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary so we can read out of it
    FILE *dict = fopen(dictionary, "r");

    // Check to make sure the file opens
    if (dict == NULL)
    {
        return false;
    }

    // Read each word out of the dictionary into the table, using the hash function defined above
    while ((fscanf(dict, "%s", buffer)) != EOF)
    {
        // Assign return value of hash function to variable for convenience
        unsigned int index = hash(buffer);

        // Add our first node to the appropriate index (as indicated by hash function)
        if (table[index] == NULL)
        {
            node *tmp = malloc(sizeof(node));
            if (tmp == NULL)
            {
                fclose(dict);
                loaded = false;
                return false;
            }

            strcpy(tmp->word, buffer);
            tmp->next = NULL;

            table[index] = tmp;
        }

        // Add subsequent nodes to the beginning of list
        else
        {
            node *tmp1 = malloc(sizeof(node));
            if (tmp1 == NULL)
            {
                fclose(dict);
                loaded = false;
                return false;
            }

            strcpy(tmp1->word, buffer);
            tmp1->next = table[index];

            table[index] = tmp1;
        }

        // Count the number of words being added to hash table (for size function)
        counter++;
    }

    fclose(dict);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // This takes the value of counter, incremented in load, and returns that value (if dict is loaded) -- otherwise returning 0
    if (loaded)
    {
        return counter;
    }

    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //
    bool count = false;

    for (int i = 0; i < N; i++)
    {
        if (free_l(table[i]) == 1)
        {
            count = true;
        }

    }

    if (count == 1)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool free_l(node *head)
{
    if (head == NULL)
    {
        return true;
    }

    else
    {
        free_l(head->next);
        free(head);
    }

    return true;
}