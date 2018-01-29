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
* Creation Date: 22-01-2018
* Copyright: (c) 2018 David Haylock""")
print("--------------------------------------")

friends_data = []

# -----------------------------------------------------
# Get a List of the Users Friends in Chat
# -----------------------------------------------------
def getUsersFriends(client):
    global friends
    # Fetches a list of all users you're currently chatting with, as `User` objects
    users = client.fetchAllUsers()

    # Get all the users name and user id
    for user in users:
        friends_data.append({
            "userid": user.uid,
            "username": user.name,
            "lastseen": ""
            })

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
    print("Difference in Time: " + str(time_diff_converted))
    return time_diff_converted

# -----------------------------------------------------
# Main Loop
# -----------------------------------------------------
def main_loop():
	
	# Do the first call
	getUsersFriends(client)

	remove_list = []

	# Loop through the results
	for index,user in enumerate(friends_data):
		newuser = client.fetchNewInfo(user["userid"])
		print(user["username"])

		# If there is message a thread between the users continue
		if newuser[0]["message_thread"] != None:

			# Get the Last Updated Time from the Users data
			last_timestamp = newuser[0]["message_thread"]["updated_time_precise"]

			# Make sure that the time stamp exists other wise remove it.
			if last_timestamp != None:
				# Convert the timestamp and insert it into the list
				t = convertTimeAndDifference(last_timestamp)
				friends_data[index]["lastseen"] = t
			else:
				# Other wise set it to be none
				friends_data[index]["lastseen"] = "Never"
		else:
			print("User is Irrelevant")
			remove_list.append(index)

	# Remove the null users from the list
	for index in sorted(remove_list,reverse=True):
		del friends_data[index]
	
	# Starting Main Loop
	print("/n")
	
	# Main Loop
	while True:
		for i in range(0,len(friends_data)):
			print(friends_data[i]["username"])
			print(friends_data[i]["lastseen"])
			time.sleep(1)

# -----------------------------------------------------
# Main Program
# -----------------------------------------------------
if __name__ == '__main__':
    client = Client("<User Email>", "<User Password>")
    try:
		main_loop()
    except KeyboardInterrupt:
        print >> sys.stderr, "\nExiting by User Request.\n"
        sys.exit(0)
