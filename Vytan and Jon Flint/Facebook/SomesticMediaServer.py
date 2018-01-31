import sys
import os
import subprocess
import logging
import json
import time
from flask import Flask, request, render_template, send_from_directory
from fbchat import Client
from fbchat.models import *

app = Flask(__name__)

print("""----------------------------------------------------------------
   ____                   __  _       __  ___       ___     
  / __/__  __ _  ___ ___ / /_(_)___  /  |/  /__ ___/ (_)__ _
 _\ \/ _ \/  ' \/ -_|_-</ __/ / __/ / /|_/ / -_) _  / / _ `/
/___/\___/_/_/_/\__/___/\__/_/\__/ /_/  /_/\__/\_,_/_/\_,_/ 
  / __/__ ________ / /  ___  ___  / /__                     
 / _// _ `/ __/ -_) _ \/ _ \/ _ \/  '_/                     
/_/  \_,_/\__/\__/_.__/\___/\___/_/\_\ """)                      
print("----------------------------------------------------------------")  
print("""* Author: David Haylock
* Project: New Talent Residencies
* Creation Date: 30-01-2018
* Copyright: (c) 2018 David Haylock""")
print("--------------------------------------")

# Empty containers
session_cookies = ""
username = ""
password = ""
user_uid = ""
searched_user_id = ""
user_name = ""

# -----------------------------------------------------
# Open the index login page
# -----------------------------------------------------
@app.route('/')
def index():
    return render_template("index.html")

# -----------------------------------------------------
# Attempt to Login
# -----------------------------------------------------
@app.route('/login',methods=['POST'])
def login(name=None):
    global client
    global user_name
    global username
    global password

    if request.method == "POST":
        # Get the Username and Password from the form
        username = request.form['emailadress']
        password = request.form['password']

        # # Load previous cookies this is for debug only!!!
        # session_cookies = json.load(open("session.json"))
        
        # Open the Client Object
        client = Client(username, password)#,session_cookies=session_cookies)
        
        # Get User Name
        newuser = client.fetchUserInfo(client.uid)[client.uid]

        # Make User Name
        user_name = "%s %s" % (newuser.first_name,newuser.last_name)
        
        # Get the Session Cookies and Save them for Later
        session_cookies = client.getSession()

        # Write the Cookies to a file
        with open("session.json","wt") as out:
            res = json.dump(session_cookies,out,sort_keys=True,indent=4,separators=(',',':'))
        
    return render_template("main.html", name=user_name)

# -----------------------------------------------------
# Look for the User
# -----------------------------------------------------
@app.route('/searchforuser',methods=['POST'])
def searchforuser(searchedusername=None):
    global searched_user_id

    # Check its a Post 
    if request.method == "POST":
        # Get the user name from the form
        searchedusername = request.form['searchedusername']
    
        # Find the User Id
        user = client.searchForUsers(searchedusername, limit=1)

        # Transfer the Variable over to the Container
        searched_user_id = user[0].uid

    return render_template("main.html",name=user_name,searchedusername=searched_user_id)

# -----------------------------------------------------
# Start the Alarm Clock
# -----------------------------------------------------
@app.route('/',methods=['POST'])
def startAlarmClock():
    
    # Check its a Post
    if request.method == "POST":
        
        # Message!
        print("About to Start the Alarm Clock")
        
        # Compile the Script Cmd
        # SomesticMediaFBAlarm.py requires Three Arguments
        # 1: -i the specified user id you want to track
        # 2: -u the user who has logged into the systems email address
        # 3: -p the user who has logged into the systems password
        cmdstr = "python SomesticMediaFBAlarm.py -i %s -u %s -p %s" % (searched_user_id,username,password)

        # Show the Command
        print(cmdstr)

        # Launch the Alarm Code
        p = os.popen(cmdstr)
        print(p)
    
    return render_template("main.html",name=user_name,searchedusername=searched_user_id,started=True)        

# -----------------------------------------------------
# Try and Logout and Clear the Session Cookies
# -----------------------------------------------------
@app.route('/logout')
def logout():
    # Clear all the data from the Flask App
    open("session.json","w+").close()
    client.logout()
    username = ""
    password = ""
    user_uid = ""
    
    return send_from_directory('./',"index.html")
