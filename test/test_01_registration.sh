#!/bin/bash

# Start the server in the background
PORT=6667
PASSWORD="testpass"

# Kill existing irc server if running on port
fuser -k $PORT/tcp 2>/dev/null
./irc $PORT $PASSWORD &
SERVER_PID=$!

# Give it a second to start
sleep 1

# Scenario 1: Connect, Pass, Nick, User
echo -e "PASS $PASSWORD\r\nNICK testuser\r\nUSER testuser 0 * :Test User\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out.txt &
NC_PID=$!

# Wait to finish
sleep 1
kill $NC_PID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

cat /tmp/irc_out.txt

if grep -q "001" /tmp/irc_out.txt; then
    echo "Test passed! Successfully registered."
    exit 0
else
    echo "Test failed!"
    cat /tmp/irc_out.txt
    exit 1
fi
