#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //prompt user for the amt of change owed, and continue to do so
    //until a number greater than or equal to 0 is chosen.
    float dollars;
    int y = 0;
    do
    {
        dollars = get_float("Change Owed: ");
    }
    while (dollars < 0);

    //convert number entered from a float (decimal) to an integer.
    int cents = round(dollars * 100);

    //this loop will end once we reach 0 cents. Until then, it will continue to
    //increase y (starting at 0) by 1 whenever a coin is shelled out.
    while (cents > 0)
    {
        if (cents >= 25) //if the # of cents is greater than or equal to 25, then a quarter can be used.
        {
             y = y + 1;
             cents = cents - 25;
        }
        if (cents >= 10 && cents < 25)// if the number of cents is >= 10 AND less than 25, a dime can be used.
        {
            y = y + 1;
            cents = cents - 10;
        }
        if (cents >= 5 && cents < 10)// same logic continues until end of loop.
        {
            y = y + 1;
            cents = cents - 5;
        }
        if (cents >= 1 && cents < 5)
        {
            y = y + 1;
            cents = cents - 1;
        }
    }
    printf("%i\n", y);
}