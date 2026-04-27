#!/bin/bash

# Scenario 7: CAP Negotiation
PORT=6673
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./irc $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Send CAP LS
echo -e "CAP LS\r\nPASS $PASSWORD\r\nNICK capusr\r\nUSER capusr 0 * :Cap Usr\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_cap.txt &
NCPID=$!

sleep 2
kill $NCPID 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

FAILED=0

# Assuming your server either replies CAP : or ignore CAP while successfully registering
# Let's just make sure registration finishes successfully despite CAP LS being prepended!
if ! grep -qi "001" /tmp/irc_out_cap.txt; then
    echo "CAP Test failed: Expected 001 Welcome during reg setup!"
    cat /tmp/irc_out_cap.txt
    FAILED=1
fi

if [ $FAILED -eq 0 ]; then
    echo "CAP Test passed!"
    exit 0
else
    exit 1
fi
