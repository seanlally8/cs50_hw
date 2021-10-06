#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

//declaring functions, described below.
int countl(string word);
int countw(string word);
int counts(string word);
float average(int number1, int number2);

//calculate readability score for text entered by user.
int main(void)
{
    //Prompt user to enter sample text here.
    string text = get_string("Text: ");

    //Use count functions (described below) to determine number of letters, words and sentences.
    int count_letters = countl(text);

    int count_words = countw(text);

    int count_sentences = counts(text);

    //Use average function (described below) to determine averages for readability test (Coleman-Liau index).
    float L = average(count_letters, count_words);

    float S = average(count_sentences, count_words);

    //Use Coleman-Liau index to determine reading level
    float index = 0.0588 * L - 0.296 * S - 15.8;
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}


//count number of letters in text entered by user.
int countl(string word)
{
    int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if ((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z'))
        {
            sum = sum + 1;
        }
    }
    return sum;
}

//count number of words in text entered by user.
int countw(string word)
{
    int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (word[i] == ' ')
        {
            sum = sum + 1;
        }
    }
    sum = sum + 1;
    return sum;
}


//count number of sentences in text entered by user.
int counts(string word)
{
    int sum = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (word[i] == '.' || word[i] == '!' || word[i] == '?')
        {
            sum = sum + 1;
        }
    }
    return sum;
}

//find average number of letters or sentences per 100 words.
float average(int number1, int number2)
{
    float avg;
    avg = (100 * number1) / (float) number2;
    return avg;
}

