#!/usr/bin/bash

# enabling ---> receiveing multi-cast message.
sudo route add -host 224.224.224.245 dev wlp0s20f3

export VSOMEIP_CONFIGURATION="./config/vsomeip.json"
export LD_LIBRARY_PATH=../../build

./server
