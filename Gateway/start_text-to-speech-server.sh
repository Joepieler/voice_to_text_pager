#!/bin/bash
#loop
while true; do
# First check if local network is online (eth0)
if route -n|grep "eth0"
then
  echo "There is a local network"
# check if there is a connection with the wifi (wlan0)
  if route -n|grep "wlan0"
  then 
    echo "There is connection with wifi"
# switch prio
    $(sudo ifmetric wlan0 100)
# check if there is a internet connection
    wget -q --spider http://google.com

    if [ $? -eq 0 ]; then
      echo "Online"
      # start python
      python3 main.py 
    else
      echo "Offline"
    fi
  fi
fi
done
