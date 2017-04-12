from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    user = db.execute("SELECT username FROM users WHERE id=:login_id", login_id=session["user_id"])
    
    data = db.execute("SELECT symbol, SUM(shares) as shares_sum, price_of_stock, SUM(total_purchase_price) as total FROM purchases WHERE purchaser=:current_user GROUP BY symbol", current_user=user[0]["username"])
    
    cash = db.execute("SELECT cash FROM users WHERE id=:login_id", login_id=session["user_id"])[0]["cash"]
    
    cash = usd(cash)
    
    index = 0
    
    for line in data:
        line["name_of_symbol"] = lookup(line["symbol"])
        line["price_of_stock"] = usd(line["price_of_stock"])
        line["total"] = usd(line["total"])
    
    data = list(filter(lambda elem: elem["shares_sum"] != 0, data))

    return render_template("index.html", data=data, cash=cash)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol_res = request.form.get("stock_symbol")
        
        shares = request.form.get("shares")
        
        if symbol_res == None or shares.strip() == "":
            return apology("symbol does not exist or your input is blank")
        elif int(shares) < 0:
            return apology("Negative number of shares!")
        
        else:
            if lookup(symbol_res) == None:
                return apology("No such symbol")
                
            total_amount = int(shares) * lookup(symbol_res)["price"]

            current_user_amount = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
            
            remainder = current_user_amount[0]["cash"] - total_amount
            
            if remainder < 0:
                return apology("Not enough money")
            
            else:
                db.execute("UPDATE users SET cash = :remainder WHERE id = :id", id=session["user_id"], remainder=remainder)
   
                db.execute("INSERT INTO purchases (purchaser, symbol, shares, price_of_stock, total_purchase_price) VALUES(:name, :sym, :shares, :price, :total)", \
                name = (db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])[0]["username"]), sym=symbol_res, shares=int(shares), price=lookup(symbol_res)["price"], \
                total=total_amount)
                
                return redirect(url_for("index"))

@app.route("/history")
@login_required
def history():
    user = db.execute("SELECT username FROM users WHERE id=:login_id", login_id=session["user_id"])
    
    data = db.execute("SELECT symbol, shares, price_of_stock, datetime(date,'localtime') as datetime FROM purchases WHERE purchaser=:current_user ORDER BY date desc", current_user=user[0]["username"])

    for line in data:
        line["price_of_stock"] = usd(line["price_of_stock"])


    return render_template("history.html", data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("stock_symbol")
        
        lookup_response = lookup(symbol )
        
        if lookup_response == None:
            return apology("Invalid symbol")
        else:
            lookup_response["price"] = usd(lookup_response["price"])
            return render_template("quoted.html", lookup_response=lookup_response)
            

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        
        name = request.form.get("username")
        password1 = request.form.get("password1")
        password2 = request.form.get("password2")
        
        name, password1, password2  = name.strip(), password1.strip(), password2.strip()
        
        if name == "" or password1 == "" or password2 == "":
            return apology("invalid username and/or password")
            
        elif password1 != password2:
            return apology("invalid username and/or password")
        
        check = db.execute("SELECT * FROM users WHERE username = :username", username=name)
        
        if len(check) != 0:
            return apology("User whith this name already exist")
        
        hashpass = pwd_context.encrypt(password1)
            
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=name, hash=hashpass)
        
        return redirect(url_for("login"))
        
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        
        symbol = request.form.get("stock_symbol").strip()
        
        shares = request.form.get("shares").strip()
        
        if symbol == "" or shares == "":
            return apology("Your input is blank")
        elif int(shares) < 0:
            return apology("Negative number of shares!")
        else:
            if lookup(symbol) == None:
                return apology("No such symbol")
            
            user = db.execute("SELECT username FROM users WHERE id=:login_id", login_id=session["user_id"])
                
            total_amount = int(shares) * lookup(symbol)["price"]

            current_user_amount = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
            
            user_shares = db.execute("SELECT SUM(shares) as shares FROM purchases WHERE symbol=:symbol AND purchaser=:current_user GROUP BY symbol", current_user=user[0]["username"], symbol=symbol)
            
            if len(user_shares) == 0:
                return apology("No shares to sell")
                
            elif (user_shares[0]["shares"] - int(shares)) < 0:
                return apology("You don't have enough shares to sell")
            
            remainder = current_user_amount[0]["cash"] + total_amount
            
            db.execute("UPDATE users SET cash = :remainder WHERE id = :id", id=session["user_id"], remainder=remainder)
   
            db.execute("INSERT INTO purchases (purchaser, symbol, shares, price_of_stock, total_purchase_price) VALUES(:name, :sym, :shares, :price, :total)", \
            name = (db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])[0]["username"]), sym=symbol, shares=-int(shares), price=lookup(symbol)["price"], \
            total=-total_amount)
            
            return redirect(url_for("index"))
    else:
        return render_template("sell.html")