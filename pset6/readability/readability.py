from cs50 import get_string


def main():
    # Prompt user to enter sample text
    text = get_string("Text: ")
    # Employ count functions to determine number of letters, words, and sentences
    countletters = count1(text)

    countwords = countw(text)

    countsentences = counts(text)

    # Use average function to determine averages
    L = average(countletters, countwords)

    S = average(countsentences, countwords)

    # Use Coleman-Liau index to determine reading level
    index = 0.0588 * L - 0.296 * S - 15.8
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {int(round(index))}")


def count1(word):
    summ = 0
    for c in word:
        if (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z'):
            summ += 1

    return summ


def countw(word):
    # Count letters
    summ = 0
    for c in word:
        if c == ' ':
            summ += 1
    summ += 1

    return summ


def counts(word):
    # Count sentences
    summ = 0
    for c in word:
        if (c == '.') or (c == '!') or (c == '?'):
            summ += 1

    return summ


def average(num1, num2):
    # Find average
    avg = ((100 * num1) / num2)
    return avg


main()