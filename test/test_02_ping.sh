#!/bin/bash

# Scenario 2: Ping/Pong
PORT=6668
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./ircserv $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

echo -e "PASS $PASSWORD\r\nNICK testping\r\nUSER testping 0 * :Test Ping\r\nPING :localhost\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_ping.txt &
NC_PID=$!

sleep 1
kill $NC_PID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

if grep -qi "PONG" /tmp/irc_out_ping.txt; then
    echo "PING Test passed!"
    exit 0
else
    echo "PING Test failed!"
    cat /tmp/irc_out_ping.txt
    exit 1
fi
