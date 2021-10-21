# This is some scratch work done as preparation for implementation of dna.py
import re
import sys
import csv

dna = []

with open(sys.argv[1]) as file:
    reader = csv.reader(file)
    for row in reader:
        dna.append(row[0])

    print(dna)

    match = re.findall("AGATC", dna[0])
    print(match)
    print(len(match))

            row["AGATC"] = int(row["AGATC"])
            row["TTTTTTCT"] = int(row["TTTTTTCT"])
            row["AATG"] = int(row["AATG"])
            row["TCTAG"] = int(row["TCTAG"])
            row["GATA"] = int(row["GATA"])
            row["TATC"] = int(row["TATC"])
            row["GAAA"] = int(row["GAAA"])
            row["TCTG"] = int(row["TCTG"])

            shorttan.append({"name": row["name"], "AGATC": row["AGATC"], "TTTTTTCT": row["TTTTTTCT"],\
                "AATG": row["AATG"], "TCTAG": row["TCTAG"], "GATA": row["GATA"], "TATC": row["TATC"],\
                "GAAA": row["GAAA"], "TCTG": row["TCTG"]})