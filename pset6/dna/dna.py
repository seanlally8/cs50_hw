import csv
import sys
import re


shorttan = []
dna = []
count = []

if not len(sys.argv) == 3:
    print("Usage: dna.py [csv file] [txt file]")
    sys.exit(1)

with open(sys.argv[1], "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        shorttan.append(row)

print(shorttan)

with open(sys.argv[2]) as file2:
    reader = csv.reader(file2)
    for row in reader:
        dna.append(row)
print()
for key in shorttan[0]:
    if key == "name":
        continue
    regex = r"(" + key + r")*"
    match = re.findall(regex, str(dna))
    print(match)
    count.append(match)

print()
