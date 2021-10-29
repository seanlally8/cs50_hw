# This is some scratch work done as preparation for implementation of dna.py
import re
import sys
import csv
import string

key = "AGATC"
jump = len(key)
count = 0
countfinal = 0

with open(sys.argv[1]) as file:
    reader = csv.reader(file)
    for row in reader:
        dna = str(row)
    print(dna)

ind = [x for x in range(len(dna)) if dna.startswith(key, x)]

print(ind)
print()

for j in range(len(ind)):
    count = 0
    print(ind[j])
    print()
    for i in dna[ind[j]::len(key)]:
        print(f"{dna.find(i)} ", end="")
        count += 1
        #if not (dna.startswith(key, dna.find(i))):
         #   break
    if count > countfinal:
        countfinal = count

print(countfinal)
