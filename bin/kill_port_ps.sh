#!/bin/bash

# Function to kill process occupying a port
kill_process_on_port() {
    local port=$1
    echo "Checking port $port..."

    # Find the PID using lsof
    pid=$(lsof -t -i :$port)

    if [ -n "$pid" ]; then
        echo "Port $port is occupied by PID $pid. Terminating process..."
        kill -9 $pid
        echo "Process $pid terminated."
    else
        echo "No process found occupying port $port."
    fi
}

kill_process_on_port $1