#! /usr/bin/env bash

echo "Running">/home/kuri/Kuri/chatbot.log
source /home/kuri/Kuri/bin/activate
source /etc/profile
#echo $PICOVOICE_API_KEY>>/home/kuri/Kuri/chatbot.log
#pip list>>/home/kuri/Kuri/chatbot.log
/home/kuri/Kuri/bin/python /home/kuri/Kuri/chatbot.py>>/home/kuri/Kuri/chatbot.log 2>&1
