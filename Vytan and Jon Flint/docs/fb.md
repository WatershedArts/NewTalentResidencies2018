Facebook Project
==

### Overview



### Requirements

You'll need Python 2.7.

and the following packages:

```
sudo pip install fbchat python-dateutil
```

When its installed you'll need to add a new function to the Client Class.

So On Mac navigate to your python package folder.

```
cd /Library/Python/2.7/site-packages/fbchat
```

On Raspberry Pi Navigate to the following folder.

```
cd /usr/local/lib/python2.7/dist-packages/fbchat/client.py
```

Then run the following commands

```
sudo nano client.py
```

About halfway down add the following function:

```
def fetchNewInfo(self, *thread_ids):
	queries = []
	for thread_id in thread_ids:
		queries.append(GraphQL(doc_id='1386147188135407', params={
			'id': thread_id,
			'message_limit': 0,
			'load_messages': False,
			'load_read_receipts': False,
			'before': None
		}))
	
	j = self.graphql_requests(*queries)
	return j
```

CTRL + x to Save

Then

```
sudo rm *.pyc
sudo python -m compileall .
```

This will add the changes into the code.

When given a userid the following code should return a similar dictionary.

```
{
  "message_thread": {
    "participant_add_mode_as_string": "",
    "pdated_time_precise": "1516659777209",
    "privacy_mode": 1,
    "thread_key": {
      "legacy_thread_id": "",
      "other_ser_id": "<number>",
      "thread_fbid": ""
    },
    "last_message": {
      "nodes": [
        {
          "commerce_message_type": "",
          "sticker": "",
          "snippet": "<String>",
          "blob_attachments": [
            
          ],
          "extensible_attachment": "",
          "message_sender": {
            "messaging_actor": {
              "id": "<number>"
            }
          },
          "timestamp_precise": "1516659777209"
        }
      ]
    },
    "image": "",
    "rtc_call_data": {
      "call_state": "NO_ONGOING_CALL",
      "initiator": "",
      "server_info_data": ""
    },
    "related_page_thread": "",
    "messages_cont": <number>,
    "approval_mode": "",
    "all_participants": {
      "nodes": [
        {
          "messaging_actor": {
            "id": "<number>",
            "__typename": "ser"
          }
        },
        {
          "messaging_actor": {
            "id": "<number>",
            "__typename": "ser"
          }
        }
      ]
    },
    "mte_ntil": "",
    "thread_qee_enabled": "False",
    "thread_qee_metadata": "",
    "event_reminders": {
      "nodes": [
        
      ]
    },
    "is_viewer_sbscribed": "True",
    "folder": "INBOX",
    "joinable_mode": {
      "link": "",
      "mode": "0"
    },
    "mentions_mte_mode": "MENTIONS_NOT_MTED",
    "ephemeral_ttl_mode": 0,
    "cstomization_enabled": "True",
    "cstomization_info": "",
    "reactions_mte_mode": "REACTIONS_NOT_MTED",
    "montage_thread": "",
    "associated_object": "",
    "thread_admins": [
      
    ],
    "name": "",
    "has_viewer_archived": "False",
    "last_read_receipt": {
      "nodes": [
        {
          "timestamp_precise": "1440884514408"
        }
      ]
    },
    "thread_type": "ONE_TO_ONE",
    "nread_cont": 2,
    "is_pin_protected": "False",
    "cannot_reply_reason": ""
  }
}
```
