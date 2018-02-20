import sys
import logging
import json
import time
import threading
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

user_last_seen = ""
user_last_seen_timestamp = ""

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
# When We Launch get the Last Time So there is something to Show
# -----------------------------------------------------
def onStartGetLastSeenTime(client,user_id):
    global user_last_seen
    global user_last_seen_timestamp
    # Get the users info based on the uid
    user = client.fetchNewInfo(user_id)

    # Check if the message thread is active
    if user[0]["message_thread"] != None:

        # Get the Last Updated Time from the Users data
        last_timestamp = user[0]["message_thread"]["updated_time_precise"]
        user_last_seen_timestamp = last_timestamp

        # Keep a reference to the last timestamp
        user_last_seen_blank = last_timestamp

        # Make sure that the time stamp exists other wise remove it.
        if last_timestamp != None:
            # Convert the timestamp and insert it into the list
            t = convertTimeAndDifference(int(last_timestamp) / 1000 )
            user_last_seen = t
        else:
            # Other wise set it to be none
            user_last_seen = ""
        
        print(user_last_seen)


# -----------------------------------------------------
# Thread for updating the Hardware
# -----------------------------------------------------
class HardwareThread(object):


    def __init__(self,interval=1):
        self.interval = interval
        thread = threading.Thread(target=self.run,args=())
        thread.daemon = True
        thread.start()

    def run(self):
        global user_last_seen
        global user_last_seen_timestamp
        while True:
            tm = 0
           
            if len(str(user_last_seen_timestamp)) == 13:
                tm = convertTimeAndDifference(int(user_last_seen_timestamp) / 1000)
            else: 
                tm = convertTimeAndDifference(int(user_last_seen_timestamp))

            print(user_last_seen_timestamp) 
            print(tm)
            time.sleep(self.interval)

# -----------------------------------------------------
# Listening Client
# -----------------------------------------------------
class ListeningClient(Client):
    def onUnknownMesssageType(self,msg=None):
        global user_id
        global user_last_seen
        global user_last_seen_timestamp
        
        if msg.get('type') == 'buddylist_overlay' and msg.get('overlay'):
            for userid, data in msg['overlay'].items():
                print('{} changed their activeness status to {}. The data was: {}'
                    .format(user_id, data.get('a'), data))

                active_status = data.get('a')

                if userid == user_id:
                    
                    if data.get('la') != None:
                        print("We have lift off")
                        user_last_seen_timestamp = data.get('la')
                        user_last_seen = convertTimeAndDifference(user_last_seen_timestamp)
                    else:
                        user_last_seen_timestamp = int(time.time())
                        user_last_seen = convertTimeAndDifference(user_last_seen_timestamp)
                      
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
    client = ListeningClient(username, password,session_cookies=session_cookies)

    # Grab a Timestamp just to start the system off
    onStartGetLastSeenTime(client,user_id)

    example = HardwareThread()

    try:
        client.listen()
    except KeyboardInterrupt:
        print >> sys.stderr, "\nExiting by User Request.\n"
        sys.exit(0)