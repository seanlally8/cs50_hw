#include <cs50.h>
#include <stdio.h>
#include<math.h>

int main(void)
{
    //Prompt user for starting population size,
    //and continue to do so until a number greater than 9 is chosen.
    int start_size;
    do
    {
        start_size = get_int("Population size: ");
    }
    while (start_size < 9);

    //Prompt user for ending population size,
    //and continue to do so until a number greater
    //than or equal to the start size is chosen.
    int end_size;
    do
    {
        end_size = get_int("End population size: ");
    }
    while (end_size < start_size);


    //Outcomes: if end size is equal to start size then print # of years as 0.
    //Otherwise, determine the number of years using the pre-given formula.
    if (end_size == start_size)
    {
        printf("Years: 0\n");
    }
    else
    {
        int y = 0;
        float born;
        int born_again;
        float dead;
        int dead_again;
        int eoy;
        do
        {
            //Run this formula until the end of year (eoy) amt
            //is greater than or equal to the end size.
            born = start_size / 3;
            born_again = trunc(born);
            dead = start_size / 4;
            dead_again = trunc(dead);
            eoy = start_size + born_again - dead_again;
            start_size =  eoy;
            y = y + 1;
        }
        while (eoy < end_size);
        printf("Years: %i\n", y);
    }
}