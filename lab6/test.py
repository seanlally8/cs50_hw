import csv
import sys
import random


teams = []

with open(sys.argv[1], "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        row["rating"] = int(row["rating"])
        teams.append({"team": row["team"], "rating": row["rating"]})

w = teams

print(w)
