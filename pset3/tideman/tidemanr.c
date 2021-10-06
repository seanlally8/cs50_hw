#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    for (int x = 0; x < candidate_count; x++)
    {
        for (int y = 0; y < candidate_count; y++)
        {
            printf("Preference %i over %i: %i\n", x, y, preferences[x][y]); //temp debug print out to test global preference int. Success!! AND to print out preference strength to help with sort_pairs function
        }
    }

    add_pairs();
    for (int i = 0; i < pair_count; i++)
    {
        printf("%i, %i\n", pairs[i].winner, pairs[i].loser); //temp print out to test add_pairs Success!
    }
    sort_pairs();
    for (int i = 0; i < pair_count; i++)
    {
        printf("%i, %i\n", pairs[i].winner, pairs[i].loser); //temp print out to test sort_pairs Success!
    }
    lock_pairs();
     for (int i = 0; i < pair_count; i++)
    {
        if (locked[pairs[i].winner][pairs[i].loser] == 1) //temp print out to test lock function
        printf("pair %i is true\n", i);
        if (locked[pairs[i].winner][pairs[i].loser] == 0)
        printf("pair %i is false\n", i);
    }
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
            {
                if (i == j)
                {
                    preferences[ranks[i]][ranks[j]] = 0;
                }
                else
                {
                    preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
                }
            }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = candidate_count*(candidate_count - 1)/2;
    int n = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
                if (preferences[i][j] == preferences[j][i])
                {
                    pair_count--;
                    continue;
                }
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[(pair_count + n) - pair_count].winner = i;
                    pairs[(pair_count + n) - pair_count].loser = j;
                }
                if (preferences[i][j] < preferences[j][i])
                {
                    pairs[(pair_count + n) - pair_count].winner = j;
                    pairs[(pair_count + n) - pair_count].loser = i;
                }
                n = n + 1;
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner] < preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner])
        {
            pairs[pair_count] = pairs[i];
            pairs[i] = pairs[i + 1];
            pairs[i + 1] = pairs[pair_count];
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
       locked[pairs[i].winner][pairs[i].loser] = true;

        for (int j = 0, n = pairs[i].loser; j < pair_count || locked[pairs[i].winner][pairs[i].loser] == 0; j++)
        {

            //If this condition is satisfied, then a cycle has been created
            if (n == pairs[i].winner)
            {
               locked[pairs[i].winner][pairs[i].loser] = false;
               break;
            }

            if (pairs[j].winner == n && locked[pairs[j].winner][pairs[j].loser] == 1)
            {
                n = pairs[j].loser;
                for (int m = 0; m < pair_count; m++)
                {
                    if (pairs[m].winner == n && locked[pairs[m].winner][pairs[m].loser] == 1)
                    {
                        n = pairs[m].loser;
                        m = 0;

                        if (n == pairs[i].winner)
                        {
                            locked[pairs[i].winner][pairs[i].loser] = false;
                            break;
                        }
                        if (m == pair_count - 1)
                        {
                            n = pairs[i].loser;
                        }
                    }
                }
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //populate an array called 'loser' with a number not found in the candidates[] array. Since candidate_count will always be one
    //greater than the uppermost limit of candidates, we can just use that. Thus, given a candidate_count of 5, the loser array
    //will look like this: [5,5,5,5,5]
    int loser[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
         loser[i] = candidate_count;
    }

    //Now, we will look at all the locked[][] arrays that are true (because our winner will not be on this list) and move the losers
    //to their corresponding slot in the loser[] array. So, if 2 is the loser, it will be moved to loser[2], and if 3 is a loser
    //it will be moved to loser[3] etc. We don't care that numbers are overwritten, since we just want to know which number didn't
    //show up as a loser in the list of true locked[][] arrays.
    for (int j = 0; j < pair_count; j++)
    {
        if (locked[pairs[j].winner][pairs[j].loser] == 1)
        {
            loser[pairs[j].loser] = pairs[j].loser;
        }
    }

    //Since the winner did not show up in the loser[] array, we will look for the index with a value equal to the candidate_count, as
    //this value has not been overwritten. As per our example, the loser array would look like this if 3 is this winner: [0,1,2,5,4].
    //Since 5 is at index 3, we know that candidate #3 is the winner. Th
    for (int m = 0; m < candidate_count; m++)
    {
         if (loser[m] == candidate_count)
         {
             printf("%s", candidates[m]);
         }
    }
    return;
}

