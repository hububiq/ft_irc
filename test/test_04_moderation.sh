#!/bin/bash

# Scenario 4: Moderation (MODE, TOPIC)
PORT=6670
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./irc $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

echo -e "PASS $PASSWORD\r\nNICK testmod\r\nUSER testmod 0 * :Test Mod\r\nJOIN #modchan\r\nMODE #modchan +t\r\nTOPIC #modchan :new topic\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_mod.txt &
NC_PID=$!

sleep 1
kill $NC_PID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

if grep -qi "MODE" /tmp/irc_out_mod.txt || grep -qi "TOPIC" /tmp/irc_out_mod.txt; then
    echo "MODERATION Test passed!"
    exit 0
else
    echo "MODERATION Test failed!"
    cat /tmp/irc_out_mod.txt
    exit 1
fi