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
* Creation Date: 31-01-2018
* Copyright: (c) 2018 David Haylock""")
print("--------------------------------------")

# -----------------------------------------------------
# Get the Time Difference
# -----------------------------------------------------
def convertTimeAndDifference(fb_timestamp):
    # Comes in, in precise millis
    fb_time = int(fb_timestamp) / 1000

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
    #print("Difference in Time: " + str(time_diff_converted))
    return time_diff_converted


# -----------------------------------------------------
# Get all the users and loop through
# -----------------------------------------------------
def specific_user_loop(user_id):

	# Blank Container
	user_last_seen_blank = ""
	user_last_seen = ""

	# Get the users info based on the uid
	user = client.fetchNewInfo(user_id)

	# Check if the message thread is active
	if user[0]["message_thread"] != None:

		# Get the Last Updated Time from the Users data
		last_timestamp = user[0]["message_thread"]["updated_time_precise"]
		
		# Keep a reference to the last timestamp
		user_last_seen_blank = last_timestamp

		# Make sure that the time stamp exists other wise remove it.
		if last_timestamp != None:
			# Convert the timestamp and insert it into the list
			t = convertTimeAndDifference(last_timestamp)
			user_last_seen = t
		else:
			# Other wise set it to be none
			user_last_seen = ""

	print("\n")

	# Main Loop
	while True:
		# Keep doing the conversion
		user_last_seen = convertTimeAndDifference(user_last_seen_blank)
		
		# Change this out for your LCD Display Code
		# print(user_last_seen)
		print("")

		# To Parse the Last Seen Data
		print("Years " + str(user_last_seen.years))
		print("Months " + str(user_last_seen.months))
		print("Days " + str(user_last_seen.days))
		print("Hours " + str(user_last_seen.hours))
		print("Minutes " + str(user_last_seen.minutes))
		print("Seconds " + str(user_last_seen.seconds))
		print("Microseconds " + str(user_last_seen.microseconds))

		# Change the Delay Time
		time.sleep(5)

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
    session_cookies = json.load(open("session.json"))
        
    # Open the Client Object
    client = Client(username, password,session_cookies=session_cookies)

    try:
		specific_user_loop(user_id)
    except KeyboardInterrupt:
        print >> sys.stderr, "\nExiting by User Request.\n"
        sys.exit(0)