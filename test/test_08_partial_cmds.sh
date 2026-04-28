#!/bin/bash

# Scenario 8: Partial Commands and Concurrency
PORT=6674
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./ircserv $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Client 1: Send partial command, wait, finish it
(
    echo -n -e "PASS $PASSWORD\r\nNICK part"
    sleep 2
    echo -e "usr\r\nUSER partusr 0 * :Part Usr\r\nQUIT :bye\r\n"
) | nc localhost $PORT > /tmp/irc_out_part.txt &
NC1=$!

# Client 2: Connect during the sleep and successfully register
sleep 0.5
echo -e "PASS $PASSWORD\r\nNICK otherusr\r\nUSER otherusr 0 * :Other Usr\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_other.txt &
NC2=$!

sleep 3
kill $NC1 $NC2 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

FAILED=0

# Client 1 should successfully register as 'partusr'
if ! grep -qi "001" /tmp/irc_out_part.txt; then
    echo "PARTIAL_CMD Test failed: Client 1 didn't complete registration!"
    cat /tmp/irc_out_part.txt
    FAILED=1
fi

# Client 2 should successfully register
if ! grep -qi "001" /tmp/irc_out_other.txt; then
    echo "PARTIAL_CMD Test failed: Client 2 blocked by partial!"
    cat /tmp/irc_out_other.txt
    FAILED=1
fi

if [ $FAILED -eq 0 ]; then
    echo "PARTIAL_CMD Test passed!"
    exit 0
else
    exit 1
fi
