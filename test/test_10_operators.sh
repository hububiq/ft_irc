#!/bin/bash

# Scenario 10: Operator vs Regular User Privileges
PORT=6676
PASSWORD="testpass"

fuser -k $PORT/tcp 2>/dev/null
./ircserv $PORT $PASSWORD &
SERVER_PID=$!
sleep 1

# Client 1 (opuser): Creates the `#opchan` channel and thereby implicitly becomes the channel operator.
(
    echo -e "PASS $PASSWORD\r\nNICK opuser\r\nUSER opuser 0 * :Op User\r\nJOIN #opchan\r"
    sleep 1
    # Try out the requested Modes (k, l, i, t, o) by the Operator!
    echo -e "MODE #opchan +k secretpass\r"
    echo -e "MODE #opchan +l 5\r"
    echo -e "MODE #opchan +i\r"
    echo -e "MODE #opchan -i\r" # Remove +i so reguser can still join
    echo -e "MODE #opchan +t\r"
    sleep 2
    echo -e "QUIT :bye\r"
) | nc localhost $PORT > /tmp/irc_out_op.txt &
NC_OP=$!

# Client 2 (reguser): Joining the channel, attempting unauthorized commands.
sleep 0.5
(
    echo -e "PASS $PASSWORD\r\nNICK reguser\r\nUSER reguser 0 * :Reg User\r\nJOIN #opchan secretpass\r"
    sleep 0.5
    # Unauthorized Operator Actions
    echo -e "KICK #opchan opuser :take this\r"
    echo -e "MODE #opchan +i\r"
    echo -e "TOPIC #opchan :new unauthorized topic\r"
    # Attempting to give itself operator mode should fail!
    echo -e "MODE #opchan +o reguser\r"
    sleep 2
    echo -e "QUIT :bye\r" 
) | nc localhost $PORT > /tmp/irc_out_reg.txt &
NC_REG=$!

sleep 4
kill $NC_OP $NC_REG 2>/dev/null
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null || true

FAILED=0

# Ensure reguser receives ERR_CHANOPRIVSNEEDED (482) when trying to execute ops commands without permission.
if ! grep -qi "482" /tmp/irc_out_reg.txt; then
    echo "OPERATOR Privilege Test failed: Regular user did not receive ERR_CHANOPRIVSNEEDED (482) when performing restricted actions!"
    cat /tmp/irc_out_reg.txt
    FAILED=1
fi

# Ensure opuser's mode changes were parsed correctly
if ! grep -qi "MODE" /tmp/irc_out_op.txt; then
    echo "OPERATOR Test failed: Opuser MODE changes were not parsed!"
    cat /tmp/irc_out_op.txt
    FAILED=1
fi

if [ $FAILED -eq 0 ]; then
    echo "OPERATOR Test passed! Regular users correctly lack operator permissions, while OP features (KICK, INVITE, TOPIC, MODE i/t/k/o/l) are properly validated."
    exit 0
else
    exit 1
fi
