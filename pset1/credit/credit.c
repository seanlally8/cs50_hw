#include <cs50.h>
#include <stdio.h>

long sum(long length, long array[]);
long remain_sum(long length, long array[]);

// this contains all calculations for determining credit card validity.
//this code has been updated (from credit_old) to include array syntax, as taught in Week 2
int main(void)
{
    //prompt user for cc#
    long ccnumber;
    ccnumber = get_long("Please enter your credit card number: ");

    //begin main algorithm. convert ccnumber into separate digits
    long ccnumber1 = ccnumber;
    long digit[16];
    for (int i = 0; i < 16; i++)
    {
        digit[i] = ccnumber1 % 10;
        ccnumber1 = (ccnumber1 - digit[i]) / 10;
    }

    //multiply every other digit (starting on the right) by 2
    long output[8];
    int n = 1;
    for (int i = 0; i < 8; i++)
    {
        output[i] = digit[i + n] * 2;
        n = n + 1;
    }

    //convert preceding products into separate digits to be added.
    long digit_sum[16];
    int x = 0;
    for (int i = 0; i < 16; i++)
    {
        digit_sum[i] = output[x] % 10; // use modelo to parse out digits, where necessary
        output[x] = (output[x] - digit_sum[i]) / 10;
        i = i + 1;
        // if output[x] was 12 then this would be capturing the "1" digit. If the initial output[x] of this
        //loop was a single digit, then this second half will just give a 0 digit.
        digit_sum[i] = output[x] % 10;
        output[x] = (output[x] - digit_sum[i]) / 10;
        x = x + 1;
    }

    //add digits from preceding section
    long products_digits_sum = sum(16, digit_sum);

    //add digits that haven't been summed yet.
    long remaining_digits_sum = remain_sum(14, digit);

    //add the previous sums
    long final_sum = products_digits_sum + remaining_digits_sum;

    //the penultimate part of the algorithm: move the final digit into a variable called final_modulo
    long final_modulo = final_sum % 10;


    //in the below, we are checking to see whether the cc# falls within the ranges
    //apportioned to each company AND to see if the final digit is equal to 0.
    // failing these, we know that the cc# is invalid for our purposes.
    if (ccnumber >= 370000000000000 && ccnumber < 380000000000000 && final_modulo == 0)
    {
        printf("AMEX\n");
    }
    else if (ccnumber >= 5100000000000000 && ccnumber < 5600000000000000 && final_modulo == 0)
    {
        printf("MASTERCARD\n");
    }
    else  if ((ccnumber >= 4000000000000 && ccnumber < 5000000000000 && final_modulo == 0)

              || (ccnumber >= 4000000000000000 && ccnumber < 5000000000000000 && final_modulo == 0))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}

//defining sum function so that we can add all the digit_sum digits.
long sum(long length, long array[])
{
    long sum = 0;
    for (int i = 0; i < length; i++)
    {
        sum = sum + array[i];
    }
    return sum;
}

//defining the sum function so that we can add the remaining digits (the ones that haven't been counted yet)
long remain_sum(long length, long array[])
{
    long sum = 0;
    for (int i = 0; i <= length; i = i + 2)
    {
        sum = sum + array[i];
    }
    return sum;
}

