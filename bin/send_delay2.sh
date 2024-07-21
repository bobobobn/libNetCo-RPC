#!/bin/bash

# Function to send a request to a server
send_request() {
    local port=$1
    local value=$2
    local response=$(echo -n $value | nc 127.0.0.1 $port)
}

# Send requests to the three servers
send_request 11345 9 &
send_request 11346 3 &
send_request 11347 1 &