import csv
from cs50 import SQL
import sys

# we can also use the touch command in cli to make file
open("shows.db", "w").close()

# Opens otherwise empty file above,
db = SQL("sqlite:///shows.db")

#Creates the two tables from notes
db.execute("CREATE TABLE shows (id Integer, title TEXT, PRIMARY KEY(id))")
db.execute("CREATE TABLE genres (show_id INTEGER, genre TEXT, FOREIGN(show_id) REFERENCES shows(id)")


title = input("Title: ").strip().upper()

with open(sys.argv[2], "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        title = row["title"].strip().upper()

        # ? is same as %s in C or {} in Python it turns out libraries will often return a value for methods like .execute below
        id = db.execute("INSERT INTO shows (title) VALUES(?)", title)
        for genre in row["genres"].split(", "):
            db.execute("INSERT INTO genres (show_id, genre) VALUES(?, ?)", id, genre)