from flask import Flask, render_template, request

app = Flask(__name__) # __name__ refers to name of current file, this line says "flask, turn current file into app"

# This (@) is called a decorator -- it applies one function to another. So we apply app (^) to index (\/)
@app.route("/", methods=["GET", "POST"])
def index():
    # We can add some if conditions to use the same route for two separate pages/ html files
    if request.method == "GET":
        return render_template("index.html") # variable to pass into index.html = predefined variable
    if request.method == "POST":
        name = request.form.get("name", "world")
        return render_template("greet.html", name=name)

#@app.route("/greet", methods=["POST"]) # We can use post to hide user information deeper in the "envelope" of the http request
#def greet():
     # second argument is "default" and the first is the argument to be searched for in the url e.g. www.example.com/?name=Sean <-- this final part of the url is
     # request.args, is for get requests, request.form, is for post requests (bad name!)
     # Use post when collecting personal information, so it doesn't show in url, but is burried in envelope instead.
 #   name = request.form.get("name", "world")
  #  return render_template("greet.html", name=name)