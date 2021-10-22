# This is some scratch work done as preparation for implementation of dna.py
import re
import sys
import csv

key = "AGATC"
with open(sys.argv[1]) as file:
    reader = csv.reader(file)
    for row in reader:
        dna = str(row)
    print(dna)

ind = [x for x in range(len(dna)) if dna.startswith(key, x)]

for j in ind:
    for i in range(len(dna[ind[j]:])):
        if not (dna.startswith(key, i)):
            break
        print(i, end="")
        i += key

#print(ind)