#!/bin/bash

# Scenario 5: User Management (INVITE, KICK)
PORT=6671
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./irc $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

echo -e "PASS $PASSWORD\r\nNICK admin\r\nUSER admin 0 * :Admin User\r\nJOIN #kickchan\r\nINVITE guest #kickchan\r\nKICK #kickchan guest :bye!\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_kick.txt &
NC_PID=$!

sleep 1
kill $NC_PID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

if grep -qi "KICK" /tmp/irc_out_kick.txt || grep -qi "401" /tmp/irc_out_kick.txt; then
    echo "USER_MGMT Test passed!"
    exit 0
else
    echo "USER_MGMT Test failed!"
    cat /tmp/irc_out_kick.txt
    exit 1
fi