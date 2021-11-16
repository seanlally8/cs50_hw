import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    purchases = db.execute("SELECT stock_symbol, num_of_shares FROM totals WHERE trackuser = ?", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = cash[0]["cash"]
    total = cash
    for purchase in purchases:
        current = lookup(purchase["stock_symbol"])
        purchase["price"] = current["price"]
        total += purchase["price"] * purchase["num_of_shares"]
    return render_template("index.html", purchases=purchases, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide stock symbol", 403)
        elif not lookup(symbol):
            return apology("invalid stock symbol", 403)

        shares = request.form.get("shares")
        try:
            int(shares)
        except ValueError:
            return apology("please select a positive integer", 403)
        if int(shares) <= 0:
            return apology("must be a positive integer", 403)

        stock = lookup(request.form.get("symbol"))
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        if stock["price"] * int(shares) > cash[0]["cash"]:
            return apology("insufficient funds", 403)

        if cash[0]["cash"] >= (stock["price"] * int(shares)):
            db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash[0]["cash"] - (stock["price"] * int(shares))), session["user_id"])
            db.execute("INSERT INTO purchases (trackuser, num_of_shares, stock_symbol, price, time_of_purchase) VALUES (?, ?, ?, ?, datetime('now'))", session["user_id"], int(shares), symbol, stock["price"])

            symbol_test = db.execute("SELECT * FROM totals WHERE stock_symbol = ? AND trackuser = ?", symbol, session["user_id"])
            if len(symbol_test) != 1:
                db.execute("INSERT INTO totals (trackuser, stock_symbol, num_of_shares) VALUES (?, ?, ?)", session["user_id"], symbol, int(shares))
            else:
                num_of_shares = db.execute("SELECT num_of_shares FROM totals WHERE stock_symbol = ?", symbol)
                db.execute("UPDATE totals SET num_of_shares = ? WHERE trackuser = ? AND stock_symbol = ?", (int(shares) + num_of_shares[0]["num_of_shares"]), session["user_id"], symbol)
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))
        return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 403)

        rows = db.execute("SELECT username FROM users WHERE username = ?", request.form.get("username"))
        if len(rows) > 0:
            return apology("username already exists", 403)

        elif not request.form.get("password"):
            return apology("must provide password", 403)
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 403)
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("your passwords don't match", 403)

        hashpassword = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hashpassword)

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        stocks = db.execute("SELECT stock_symbol FROM purchases WHERE trackuser = ? GROUP BY stock_symbol", session["user_id"])
        return render_template("sell.html", stocks=stocks)

    if request.method == "POST":
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        stock = lookup(request.form.get("symbol"))

        if not symbol:
            return apology("no symbol selected", 403)

        symbol_test = db.execute("SELECT * FROM totals WHERE stock_symbol = ? AND trackuser = ?", symbol, session["user_id"])
        if len(symbol_test) != 1:
            return apology("you don't own any of these shares", 403)

        try:
            if int(shares) <= 0:
                return apology("please select a positive integer", 403)
        except ValueError:
            return apology("please select a positive integer", 403)

        num_of_shares = db.execute("SELECT num_of_shares FROM totals WHERE trackuser = ? AND stock_symbol = ?", session["user_id"], symbol)
        num_of_shares = num_of_shares[0]["num_of_shares"]
        if num_of_shares < int(shares):
            return apology("you do not have enough shares for this transaction", 403)
        else:
            new_shares_total = num_of_shares - int(shares)
            db.execute("INSERT INTO sales (trackuser, num_of_shares, stock_symbol, price, time_of_sale) VALUES (?, ?, ?, ?, datetime('now'))", session["user_id"], int(shares), symbol, stock["price"])
            db.execute("UPDATE totals SET num_of_shares = ? WHERE trackuser = ? AND stock_symbol = ?", new_shares_total, session["user_id"], symbol)
            income = int(shares) * stock["price"]

            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            # multiply # of shares sold by current price and add the product to "cash" in "users" table
            db.execute("UPDATE users SET cash = ? WHERE id = ?", income + cash[0]["cash"], session["user_id"])

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
