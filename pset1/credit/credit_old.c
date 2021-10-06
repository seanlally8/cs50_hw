#include <cs50.h>
#include <stdio.h>


// this contains all calculations for determining credit card validity.
int main(void)
{
    //prompt user for cc#
    long ccnumber;
    long ccnumber1;
    ccnumber = get_long("Please enter your credit card number: ");

    //begin main algorithm. convert ccnumber into separate digits
    long digit1 = ccnumber % 10;
    ccnumber1 = (ccnumber - digit1) / 10;
    long digit2 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit2) / 10;
    long digit3 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit3) / 10;
    long digit4 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit4) / 10;
    long digit5 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit5) / 10;
    long digit6 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit6) / 10;
    long digit7 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit7) / 10;
    long digit8 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit8) / 10;
    long digit9 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit9) / 10;
    long digit10 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit10) / 10;
    long digit11 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit11) / 10;
    long digit12 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit12) / 10;
    long digit13 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit13) / 10;
    long digit14 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit14) / 10;
    long digit15 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit15) / 10;
    long digit16 = ccnumber1 % 10;
    ccnumber1 = (ccnumber1 - digit16) / 10;

    //multiply every other digit (starting on the right) by 2 before adding their digits
    long output1 = digit2 * 2;
    long output2 = digit4 * 2;
    long output3 = digit6 * 2;
    long output4 = digit8 * 2;
    long output5 = digit10 * 2;
    long output6 = digit12 * 2;
    long output7 = digit14 * 2;
    long output8 = digit16 * 2;


    //convert preceding products into separate digits to be added.
    long digit_sum1 = output1 % 10;
    output1 = (output1 - digit_sum1) / 10;
    long digit_sum2 = output1 % 10;
    output1 = (output1 - digit_sum2) / 10;
    long digit_sum3 = output2 % 10;
    output2 = (output2 - digit_sum3) / 10;
    long digit_sum4 = output2 % 10;
    output2 = (output2 - digit_sum4) / 10;
    long digit_sum5 = output3 % 10;
    output3 = (output3 - digit_sum5) / 10;
    long digit_sum6 = output3 % 10;
    output3 = (output3 - digit_sum6) / 10;
    long digit_sum7 = output4 % 10;
    output4 = (output4 - digit_sum7) / 10;
    long digit_sum8 = output4 % 10;
    output4 = (output4 - digit_sum8) / 10;
    long digit_sum9 = output5 % 10;
    output5 = (output5 - digit_sum9) / 10;
    long digit_sum10 = output5 % 10;
    output5 = (output5 - digit_sum10) / 10;
    long digit_sum11 = output6 % 10;
    output6 = (output6 - digit_sum11) / 10;
    long digit_sum12 = output6 % 10;
    output6 = (output6 - digit_sum12) / 10;
    long digit_sum13 = output7 % 10;
    output7 = (output7 - digit_sum13) / 10;
    long digit_sum14 = output7 % 10;
    output7 = (output7 - digit_sum14) / 10;
    long digit_sum15 = output8 % 10;
    output8 = (output8 - digit_sum15) / 10;
    long digit_sum16 = output8 % 10;
    output8 = (output8 - digit_sum16) / 10;

    printf("digit_sum7: %li\n", digit_sum7);
     printf("digit_sum11: %li\n", digit_sum11);

    //add digits from preceding section
    long products_digits_sum = digit_sum1 + digit_sum2 + digit_sum3 + digit_sum4 + digit_sum5 + digit_sum6 + digit_sum7 + digit_sum8 +
    digit_sum9 + digit_sum10 + digit_sum11 + digit_sum12 + digit_sum13 + digit_sum14 + digit_sum15 + digit_sum16;
    //add digits that haven't been summed yet.
    long remaining_digits_sum = digit1 + digit3 + digit5 + digit7 + digit9 + digit11 + digit13 + digit15;
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

            || (ccnumber >= 4000000000000000 & ccnumber < 5000000000000000 && final_modulo == 0))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}


