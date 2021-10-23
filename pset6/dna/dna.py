# A DNA profiling program
import csv
import sys


def main():
    # Global assignment of lists:
    # database (for csv file),
    # strcount (for holding values for repetitions of longest str sequence)
    database = []
    strcount = []

    # Make sure user enters correct number of arguments`
    if not len(sys.argv) == 3:
        print("Usage: dna.py [csv file] [txt file]")
        sys.exit(1)

    # Load csv into memory
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

    # Load DNA sequence into memory (as a string)
    with open(sys.argv[2]) as file2:
        reader = csv.reader(file2)
        for row in reader:
            dna = str(row)

    # Use keys (except for "name") in database[] to find number of str repetitions for each str type
    for key in database[0]:
        if key == "name":
            continue
        else:
            strcount.append(sequence(key, dna))

    # Compare repetions in DNA sequence with values listed in CSV and print name of match if match is found
    for i in range(len(database)):
        tmplist = []
        for key in database[i]:
            if key == "name":
                continue
            else:
                tmplist.append(int(database[i][key]))
        if tmplist == strcount:
            print(database[i]["name"])
            sys.exit(0)

    # If no match was found print no match
    print("No Match")
    sys.exit(1)


# Takes an str (such as AGATC) and the whole DNA sequence as input and out puts number of repetions for longest chain of strs
def sequence(key, dna):
    count = 0
    countf = 0

    reps = [x for x in range(len(dna)) if dna.startswith(key, x)]
    for i in range(len(reps)):
        count = 0
        for j in range(reps[i], len(dna), len(key)):
            if dna.startswith(key, j):
                count += 1
            else:
                break
        if count > countf:
            countf = count
    return countf


main()

