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

    add_pairs();
    sort_pairs();
    lock_pairs();
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
    pair_count = candidate_count * (candidate_count - 1) / 2;
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
        if (preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner] < preferences[pairs[i +
                1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner])
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

        int n = pairs[i].winner;
        int temp = 0;
        do
        {
            for (int j = 0; j < pair_count; j++)
            {
                if (pairs[j].loser == n && locked[pairs[j].winner][pairs[j].loser] == 1)
                {
                    n = pairs[j].winner;
                    break;
                }
                temp = j;
            }
            if (temp == pair_count - 1)
            {
                break;
            }
        }
        while (n != pairs[i].loser);

        if (pairs[i].loser == n)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
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

    //If a locked pair is marked as true that means the second dimesion (pairs[].loser) is on the receiving end of an arrow.
    //This means that person cannot be the winner. We thus put them in the loser array at the index equal to their value. the 0th candidate
    //would be placed at index 0 of the loser array.
    for (int j = 0; j < pair_count; j++)
    {
        if (locked[pairs[j].winner][pairs[j].loser] == 1)
        {
            loser[pairs[j].loser] = pairs[j].loser;
        }
    }

    //If the candidate count is 5 -- and 3 is the winner -- then the loser array will look like this [0,1,2,5,4].Thus the candidate
    //at index 3 of the candidates array will be the winner.
    for (int m = 0; m < candidate_count; m++)
    {
        if (loser[m] == candidate_count)
        {
            printf("%s\n", candidates[m]);
        }
    }
}

