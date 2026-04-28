#!/bin/bash

# Scenario 9: Kill mid-command and stop-flood-resume
PORT=6675
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./ircserv $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Client 1: Send partial command and mysteriously vanish (kill -9)
(
    echo -n -e "PASS $PASSWORD\r\nNICK deadm"
    sleep 5
) | nc localhost $PORT >/dev/null 2>&1 &
NC1=$!

sleep 0.5
kill -9 $NC1 2>/dev/null

# Client 2: Verify server is perfectly alive right after murder
echo -e "PASS $PASSWORD\r\nNICK newalive\r\nUSER aliveuser 0 * :Alive User\r\nQUIT :bye\r\n" | nc localhost $PORT > /tmp/irc_out_alive.txt &
NC2=$!

# Client 3 (Stop-flood-resume): Setup connection to #floodchan
echo -e "PASS $PASSWORD\r\nNICK floodtrig\r\nUSER floodtrg 0 * :Flood\r\nJOIN #floodchan\r\n" > /tmp/flood_input &
tail -f /tmp/flood_input | nc localhost $PORT > /tmp/irc_out_flood.txt &
NC3=$!

sleep 0.5
# Send STOP to Client 3! (Simulating Ctrl+Z on client device)
kill -STOP $NC3 2>/dev/null

# Now flood #floodchan from Client 2!
(
    echo -e "JOIN #floodchan\r"
    for i in {1..20}; do
        echo -e "PRIVMSG #floodchan :flood message $i\r"
    done
    sleep 2
    echo -e "QUIT :bye\r"
) | nc localhost $PORT > /dev/null &
NC4=$!

sleep 1

# Wake up Client 3 to consume flooded messages
kill -CONT $NC3 2>/dev/null
sleep 1
# Nicely shut it down
echo -e "QUIT :bye\r\n" >> /tmp/flood_input

kill $NC2 $NC3 $NC4 2>/dev/null

kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

FAILED=0

# Client 2 should be successfully processed after murder of Client 1
if ! grep -qi "001" /tmp/irc_out_alive.txt; then
    echo "KILL_MID_CMD/RESUME Test failed: Server blocked after murder!"
    cat /tmp/irc_out_alive.txt
    FAILED=1
fi

# Client 3 should have received 20 flood messages
count=$(grep -c "flood message" /tmp/irc_out_flood.txt || true)
if [ $count -ne 20 ]; then
    echo "KILL_MID_CMD/RESUME Test failed: Flooded messages dropped or blocked! Expected 20, got $count"
    cat /tmp/irc_out_flood.txt
    FAILED=1
fi

if [ $FAILED -eq 0 ]; then
    echo "KILL_MID_CMD/RESUME Test passed!"
    exit 0
else
    exit 1
fi
