#!/bin/bash

# Scenario 6: Errors and Edge Cases
PORT=6672
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./ircserv $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# 1. Invalid Password
echo -e "PASS wrongpass\r\nNICK testerr\r\nUSER testerr 0 * :Error User\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_err1.txt &
NC1=$!

# 2. Duplicate NICK
echo -e "PASS $PASSWORD\r\nNICK dupnick\r\nUSER user1 0 * :User One\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_err2_a.txt &
NC2A=$!
sleep 1
echo -e "PASS $PASSWORD\r\nNICK dupnick\r\nUSER user2 0 * :User Two\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_err2_b.txt &
NC2B=$!

sleep 2
kill $NC1 $NC2A $NC2B 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

FAILED=0

# Check Invalid Password response (ERR_PASSWDMISMATCH 464)
if ! grep -qi "464" /tmp/irc_out_err1.txt; then
    echo "ERRORS Test failed: Expected 464 Password Mismatch!"
    cat /tmp/irc_out_err1.txt
    FAILED=1
fi

# Check Duplicate NICK (ERR_NICKNAMEINUSE 433)
if ! grep -qi "433" /tmp/irc_out_err2_b.txt; then
    echo "ERRORS Test failed: Expected 433 Nickname in use!"
    cat /tmp/irc_out_err2_b.txt
    FAILED=1
fi

if [ $FAILED -eq 0 ]; then
    echo "ERRORS Test passed!"
    exit 0
else
    exit 1
fi
