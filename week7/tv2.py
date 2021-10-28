import csv
import sys

title = input("Title: ").strip().upper()

with open(sys.argv[1], "r") as file:
    reader = csv.DictReader(file)
    counter = 0
    for row in reader:
        if row["Title"].strip().upper() == title:
            counter += 1

print(counter)