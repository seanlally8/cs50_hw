from cs50 import get_int
from cs50 import get_string


def main():
    # Prompt user for cc#
    ccnumber = get_int("Please enter your credit card number: ")

    # Begin main algorithm, convert ccnumber into separate digits
    ccnumberstr = str(ccnumber)
    ccnumber1 = ccnumber
    digit = []
    for i in range(len(ccnumberstr)):
        digit.append(int(ccnumber1 % 10))
        ccnumber1 = (ccnumber1 - digit[i]) / 10

    # Multiply every other digit (starting on the right) by 2
    output = []
    n = 1
    for i in range(int(len(ccnumberstr) / 2)):
        output.append(int(digit[i + n] * 2))
        n += 1

    # Convert preceding products into separate digits to be added
    digitsum = []
    x = 0
    i = 0
    while (any(output) == True):
        digitsum.append(output[x] % 10)
        output[x] = int((output[x] - digitsum[i]) / 10)
        i += 1
        digitsum.append(output[x] % 10)
        output[x] = int((output[x] - digitsum[i]) / 10)
        x += 1
        i += 1

    # Add digits from preceding section
    productsdigitssum = summ(len(digitsum), digitsum)

    # Add digits that haven't been summed yet
    remainingdigitssum = remainsum(len(digit), digit)

    # Add the previous sums
    finalsum = productsdigitssum + remainingdigitssum

    # The punultimate part: assign the final digit to a variable
    finalmodulo = finalsum % 10

    if ccnumber >= 370000000000000 and ccnumber < 380000000000000 and finalmodulo == 0:
        print("AMEX")
    elif ccnumber >= 5100000000000000 and ccnumber < 5600000000000000 and finalmodulo == 0:
        print("MASTERCARD")
    elif (ccnumber >= 4000000000000 and ccnumber < 5000000000000 and finalmodulo == 0) or \
            (ccnumber >= 4000000000000000 and ccnumber < 5000000000000000 and finalmodulo == 0):
        print("VISA")
    else:
        print("INVALID")


def summ(length, array):
    add = 0
    for i in range(length):
        add += array[i]
    return add


def remainsum(length, array):
    add = 0
    for i in range(0, length, 2):
        add += array[i]
    return add


main()