import sys
import cs50


def main():

    # Prompt user for integer between 1 and 8, inclusive. Continue to prompt them until they select an appropriate value
    while True:
        n = cs50.get_int("Pick a number between 1 and 8, inclusive: ")
        if n >= 1 and n <= 8:
            break

    # Assign the slected value to a fixed variable, so that we can create the first triangle in pyr
    x = n

    # Draw the pyramids
    pyr(x, n)


def pyr(x, n):
    # Base case: once the passed value is 0, we can't draw any more pyramids
    if n == 0:
        return

    # Before drawing the largest row, draw a row that is 1 unit smaller than the selected size
    pyr(x, n - 1)

    # Print the row according to the value passed into the function
    print("{0}{1}  {2}".format(" " * (x - n), "#" * n,  "#" * n))


main()