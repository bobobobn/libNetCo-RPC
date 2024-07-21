#!/bin/bash

# Start the three programs in the background
./rpc_server_can_set_delay 12345 &
./rpc_server_can_set_delay 12346 &

# Wait for all background jobs to complete
wait

echo "All programs have been started."