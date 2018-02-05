import sys
import logging
import json
import time
from fbchat import Client
from fbchat.models import *
from datetime import datetime
from dateutil import relativedelta

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
* Creation Date: 05-02-2018
* Copyright: (c) 2018 David Haylock""")
print("--------------------------------------")

# -----------------------------------------------------
# Get the Time Difference
# -----------------------------------------------------
def convertTimeAndDifference(fb_timestamp):
    # Comes in, in precise millis
    fb_time = int(fb_timestamp)

    # Convert the Timestamp
    fb_converted_time = datetime.fromtimestamp(int(fb_time)).strftime("%Y-%m-%d %H:%M:%S")

    # Get the Current Timestamp
    current_timestamp = datetime.now()

    # Convert the Current Timestamp
    current_timestamp_converted = current_timestamp.strftime("%Y-%m-%d %H:%M:%S")

    # Change type to datetime
    ctd = datetime.strptime(current_timestamp_converted,"%Y-%m-%d %H:%M:%S")
    fbctd = datetime.strptime(fb_converted_time,"%Y-%m-%d %H:%M:%S")

	# Put the time into a container
    time_diff_converted = relativedelta.relativedelta(ctd,fbctd)
    return time_diff_converted

# -----------------------------------------------------
# Get the Arguments in a Dictionary
# Thanks to Dideler https://gist.github.com/dideler/2395703
# -----------------------------------------------------
def getopts(argv):
    opts = {}  # Empty dictionary to store key-value pairs.
    while argv:  # While there are arguments left to parse...
        if argv[0][0] == '-':  # Found a "-name value" pair.
            opts[argv[0]] = argv[1]  # Add key and value to the dictionary.
        argv = argv[1:]  # Reduce the argument list by copying it starting from index 1.
    return opts

# -----------------------------------------------------
# Listening Client
# -----------------------------------------------------
class ListeningClient(Client):
    def onChatTimestamp(self, buddylist, msg):
        global user_id
        print(datetime.now())
        # Loop through the list of people
        for id,time in buddylist.items():
            print("")
            
            print(str(id) + " " + str(convertTimeAndDifference(time)))
            # Check the User ID against the ones in the buddy list
            # if it matches the ids, return the timestamp and pass it through the link
            if id == user_id:
                diff = convertTimeAndDifference(time)

                # To Parse the Last Seen Data
                print("Years " + str(diff.years))
                print("Months " + str(diff.months))
                print("Days " + str(diff.days))
                print("Hours " + str(diff.hours))
                print("Minutes " + str(diff.minutes))
                print("Seconds " + str(diff.seconds))
                print("Microseconds " + str(diff.microseconds))

                # print(diff)

# -----------------------------------------------------
# Main Program
# -----------------------------------------------------
if __name__ == '__main__':
    from sys import argv
    
    # Get the Arguments
    myargs = getopts(argv)

    # If there is no user id -> exit
    if not '-i' in myargs:
        print("No User Id")
        sys.exit(0)
        
    # If there is no user name -> exit
    if not '-u' in myargs:
        print("No User Name")
        sys.exit(0)

    # If there is no password -> exit
    if not '-p' in myargs:
        print("No User Password")
        sys.exit(0)

    # Transfer the values
    user_id = myargs['-i']
    username = myargs['-u']
    password = myargs['-p']

    # Load previous cookies 
    # session_cookies = json.load(open("session.json"))
        
    # Open the Client Object
    client = ListeningClient(username, password)#,session_cookies=session_cookies)

    try:
        client.listen()
    except KeyboardInterrupt:
        print >> sys.stderr, "\nExiting by User Request.\n"
        sys.exit(0)