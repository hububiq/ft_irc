#!/bin/bash

# Scenario 3: Channels (JOIN, PRIVMSG)
PORT=6669
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./irc $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

echo -e "PASS $PASSWORD\r\nNICK testchan\r\nUSER testchan 0 * :Test Chan\r\nJOIN #mychannel\r\nPRIVMSG #mychannel :hello everyone\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_chan.txt &
NC_PID=$!

sleep 3
kill $NC_PID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

if grep -qi "JOIN" /tmp/irc_out_chan.txt; then
    echo "CHANNEL Test passed!"
    exit 0
else
    echo "CHANNEL Test failed!"
    cat /tmp/irc_out_chan.txt
    exit 1
fi
