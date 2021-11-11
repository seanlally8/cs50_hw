from cs50 import SQL
from flask import Flask, render_template, request, redirect
from flask_mail import Mail, Message

app = Flask(__name__) # __name__ refers to name of current file, this line says "flask, turn current file into app"
1`

db = SQL("sqlite:///froshims.db")

SPORTS = [
    "Dodgeball",
    "Flag Football",
    "Soccer",
    "Ultimate Frisbee",
    "Baseball",
]


# This (@) is called a decorator -- it applies one function to another. So we apply app (^) to index (\/)
@app.route("/")
def index():
    return render_template("index.html", sports=SPORTS) # This then needs to be updated in index.html with jinja/flask syntax

@app.route("/register", methods=["POST"])
def register():
    name = request.form.get("name")
    if not name:
        return render_template("error.html", message="Missing name")
    sport = request.form.get("sport")
    if not sport:
        return render_template("error.html", message="Missing sport")
    if sport not in SPORTS:
        return render_template("error.html", message="Invalid sport")

    db.execute("INSERT INTO registrants (name,sport) VALUES(?, ?)", name, sport)

    return redirect("/registrants")

@app.route("/registrants")
def registrants():
    # the problem here is that the registrants data won't persist
    registrants = db.execute("SELECT * FROM registrants")
    return render_template("registrants.html", registrants=registrants)