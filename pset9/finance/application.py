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

    # Fetch the total number of stocks owned by the user for a given stock symbol
    purchases = db.execute("SELECT stock_symbol, num_of_shares FROM totals WHERE trackuser = ?", session["user_id"])

    # Fetch the total available cash in user's account
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    # Extract the floating point value for ease of coding
    cash = cash[0]["cash"]

    # Store cash amount in variable called total so we can then add the stock value to that number for our grand total
    total = cash

    # Add current value of stocks owned by user to their current cash amount. This is our grand total
    for purchase in purchases:
        current = lookup(purchase["stock_symbol"])
        purchase["price"] = current["price"]
        total += purchase["price"] * purchase["num_of_shares"]

    # Send user to their homepage where they will now find a table with information about their stocks
    return render_template("index.html", purchases=purchases, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via GET, so we send them to a page with forms for buying stocks
    if request.method == "GET":
        return render_template("buy.html")

    # They've submitted forms on buy.html, so we process their request
    if request.method == "POST":

        # Store the value of the Stock Symbol in a variable called symbol
        symbol = request.form.get("symbol")

        # If no symbol given, ask the user to go back and provide a stock symbol
        if not symbol:
            return apology("must provide stock symbol")

        # And if there's no return value from lookup (i.e. the stock symbol does not exist) then tell the user.
        elif not lookup(symbol):
            return apology("invalid stock symbol")

        # Store the number of shares entered by user in a variable called shares
        shares = request.form.get("shares")

        # Check to make sure that the user inputs a positive integer and that they didn't try to input a string
        try:
            if int(shares) <= 0:
                return apology("must be a positive integer")
        except ValueError:
            return apology("please select a positive integer")

        # Store the requested Stock Symbol in a variable called stock
        stock = lookup(request.form.get("symbol"))

        # Fetch the user's total cash amount, so we can adjust it in accordance with the transaction
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # If the user doesn't have enough money, then inform the user that they have insufficient funds
        if stock["price"] * int(shares) > cash[0]["cash"]:
            return apology("insufficient funds")

        # If the user has enough cash to buy the stocks, then subtract the total price of the stocks from the user's cash
        if cash[0]["cash"] >= (stock["price"] * int(shares)):
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       (cash[0]["cash"] - (stock["price"] * int(shares))), session["user_id"])

            # Record the purchase in the database
            db.execute("INSERT INTO purchases (trackuser, num_of_shares, stock_symbol, price, time_of_purchase) VALUES (?, ?, ?, ?, datetime('now'))",
                       session["user_id"], int(shares), symbol, stock["price"])

            # Check to see if the symbol already exists in a table designed to track the total number of stocks for each user.
            symbol_test = db.execute("SELECT * FROM totals WHERE stock_symbol = ? AND trackuser = ?", symbol, session["user_id"])

            # If it doesn't, create a new row for that user with the stock symbol and total number of shares. Having this table makes it easier to update the stock summary table and to check
            # whether the user has any given stock type
            if len(symbol_test) != 1:
                db.execute("INSERT INTO totals (trackuser, stock_symbol, num_of_shares) VALUES (?, ?, ?)",
                           session["user_id"], symbol, int(shares))

            # Otherwise, update the number of shares for the existing row.
            else:
                num_of_shares = db.execute(
                    "SELECT num_of_shares FROM totals WHERE stock_symbol = ? AND trackuser = ?", symbol, session["user_id"])
                db.execute("UPDATE totals SET num_of_shares = ? WHERE trackuser = ? AND stock_symbol = ?",
                           (int(shares) + num_of_shares[0]["num_of_shares"]), session["user_id"], symbol)

        # Once all the operations are complete, send the user back to their homepage with an updated stock table
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve all the data from the sales and purchases tables, and order the date by the time of the transaction. Then display it in a table on the history page.
    transactions = db.execute(
        "SELECT * FROM sales UNION SELECT * FROM purchases WHERE trackuser = ? ORDER BY time_of_sale DESC", session["user_id"])
    return render_template("history.html", transactions=transactions)


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

    # If the user is sent to the quote route via GET, then just load the page with the forms needed to get a quote for any given stock symbol.
    if request.method == "GET":
        return render_template("quote.html")

    # If they arrive via POST, then check to make sure a symbol has been entered by the user, before looking up the most current information regarding the stock in question.
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not lookup(symbol):
            return apology("invalid stock symbol")
        quote = lookup(symbol)

        # Once the information is retrieved, send the user to a page containing the quote data
        return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # If the user arrives via GET, load the page with the forms needed to register for C$50 account.
    if request.method == "GET":
        return render_template("register.html")

    # If the user submits values on the register.html page, then process their information.
    if request.method == "POST":

        # If no username entered, ask them to provide one.
        if not request.form.get("username"):
            return apology("must provide username")

        # Query the database to see if the username already exists. If so, let them know so they can try again with a new username.
        rows = db.execute("SELECT username FROM users WHERE username = ?", request.form.get("username"))
        if len(rows) > 0:
            return apology("username already exists")

        # Check to make sure a password has been entered, then confirmed. And make sure the password matches the password entered into the confirmation field.
        elif not request.form.get("password"):
            return apology("must provide password")
        elif not request.form.get("confirmation"):
            return apology("must confirm password")
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("your passwords don't match")

        # Convert the password into a hash value, so that even we don't know their password.
        hashpassword = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

        # Enter that hash value into the database.
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), hashpassword)

        # Send the user to their new homepage, where they'll find an empty stock summary table.
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # If arrived via GET, load a page containing forms allowing them to sell stocks they already own.
    if request.method == "GET":

        # We need to retrieve the stock symbols they already own to correctly populate the dropdown in the sell.html page.
        stocks = db.execute("SELECT stock_symbol FROM purchases WHERE trackuser = ? GROUP BY stock_symbol", session["user_id"])
        return render_template("sell.html", stocks=stocks)

    # If arrived via POST, process the information entered by the user.
    if request.method == "POST":

        # Store the information provided by the forms in aptly named variables.
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        stock = lookup(request.form.get("symbol"))

        # If no stock symbol has been entered, let them know, so they can go back and try again.
        if not symbol:
            return apology("no symbol selected")

        # Check to see if the stock is already owned by the user.
        symbol_test = db.execute("SELECT * FROM totals WHERE stock_symbol = ? AND trackuser = ?", symbol, session["user_id"])

        # If not, let them know.
        if len(symbol_test) != 1:
            return apology("you don't own any of these shares")

        # If they enter a negative number (or 0) -- or if they try to input a string -- inform them that they must enter a positive integer.
        try:
            if int(shares) <= 0:
                return apology("please select a positive integer")
        except ValueError:
            return apology("please select a positive integer")

        # Create a variable containing the number of shares currently owned by the user, so you can
        # check to see if the number of shares entered by the user exceeds the number of shares owned by the user.
        num_of_shares = db.execute(
            "SELECT num_of_shares FROM totals WHERE trackuser = ? AND stock_symbol = ?", session["user_id"], symbol)
        num_of_shares = num_of_shares[0]["num_of_shares"]

        # If so, let them know they don't have enough shares.
        if num_of_shares < int(shares):
            return apology("you do not have enough shares for this transaction")

        # If they do have enough shares, update the database accordingly. Record the sale in the sales table, and subtract the number of shares from the existing entry in the totals table.
        else:
            new_shares_total = num_of_shares - int(shares)
            db.execute("INSERT INTO sales (trackuser, num_of_shares, stock_symbol, price, time_of_sale) VALUES (?, ?, ?, ?, datetime('now'))",
                       session["user_id"], int(shares), symbol, stock["price"])
            db.execute("UPDATE totals SET num_of_shares = ? WHERE trackuser = ? AND stock_symbol = ?",
                       new_shares_total, session["user_id"], symbol)

            # Create two variables: one containing the newly gained income (# of shares sold * the current price of the shares) and another containing the user's current cash amount.
            income = int(shares) * stock["price"]
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            # Update the user's cash amount so it includes the newly received income.
            db.execute("UPDATE users SET cash = ? WHERE id = ?", income + cash[0]["cash"], session["user_id"])

        # Once the information is processed, send the user to their homepage with the newly updated stock summary
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
