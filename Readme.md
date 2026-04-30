*This project has been created as part of the 42 curriculum by hgatarek, sandrzej, mmitkovi.*

---

# ft_irc — Internet Relay Chat Server

A fully functional IRC server built in **C++98**, written from scratch as part of the 42 school curriculum. The server handles multiple simultaneous clients over TCP/IP using a single `epoll`-based event loop, and is compatible with standard IRC clients.

---

## Description

**Internet Relay Chat (IRC)** is a text-based communication protocol that enables real-time messaging — both public (channels) and private (direct messages). This project implements the **server side** of the IRC protocol: clients connect using any standard IRC client, authenticate with a password, and can interact through channels and private messages.

The server is built around a **non-blocking, single-threaded** architecture — no forking, no threads. All I/O multiplexing is handled through a `epoll` instance, making the server efficient and deterministic under concurrent load.

Key constraints enforced:
- Written strictly in **C++98**
- **No external libraries** (no Boost, no libft)
- **No forking** — all I/O is non-blocking
- A **`epoll`** instance handles all file descriptor events

---

## Architecture

```
ServerRunner
├── Listener          — Sets up and binds the listening socket
├── EpollInitializer  — Initializes the epoll instance
└── Multiplexer       — Main event loop; dispatches events
    ├── ConnHandler         — Accepts new client connections
    ├── EpollStateManager   — Manages fd registration/modification in epoll
    └── RequestHandler      — Reads raw data from client fds; manages buffers
        ├── MessageParser       — Tokenizes and validates raw IRC messages
        └── Executor            — Dispatches parsed commands
            └── CommandHandler
                └── PASS / NICK / USER / JOIN / PRIVMSG / ...
```

**Supporting singletons / shared state:**
- `ServerDao` — Holds all server-side state: connected clients, channels, nicknames
- `ReplyFactory` — Constructs RFC-compliant numeric and string reply messages

**Connection lifecycle:**
1. `Listener` accepts a new TCP connection → fd registered with `EpollStateManager`
2. `epoll_wait` fires a read event → `RequestHandler` reads and buffers raw bytes
3. Buffer is scanned for complete `\r\n`-terminated messages → handed to `MessageParser`
4. Parsed command + arguments → `Executor` → specific `CommandHandler`
5. Replies are written back to the client fd (also through epoll write-readiness)

---

## Features

- Simultaneous multi-client support with a single `epoll` event loop
- Full client **authentication** flow (`PASS` → `NICK` → `USER`)
- **Channels** with operators and regular users
- **Operator-only commands**: `KICK`, `INVITE`, `TOPIC`, `MODE`
- **Channel modes**: `i` (invite-only), `t` (topic-restricted), `k` (key/password), `o` (operator), `l` (user limit)
- **Partial message reassembly** — handles fragmented TCP packets correctly
- RFC-compliant **numeric replies** via `ReplyFactory`
- Graceful client disconnection handling (`QUIT`, broken pipe, EOF)

---

## Implemented Commands

| Command   | Description |
|-----------|-------------|
| `CAP`     | Capability negotiation (handled to support modern clients) |
| `PASS`    | Authenticate with the server password |
| `NICK`    | Set or change a client's nickname |
| `USER`    | Set username and realname during registration |
| `PING`    | Respond to server/client keepalive pings |
| `JOIN`    | Join a channel (creates it if it does not exist) |
| `PRIVMSG` | Send a message to a channel or directly to a user |
| `TOPIC`   | View or change the topic of a channel |
| `MODE`    | Set channel modes (`i`, `t`, `k`, `o`, `l`) |
| `KICK`    | Remove a user from a channel (operator only) |
| `INVITE`  | Invite a user to a channel (operator only) |
| `QUIT`    | Disconnect from the server gracefully |

---

## Instructions

### Requirements

- A C++98-compliant compiler (`g++` or `clang++`)
- Linux (epoll is Linux-specific)

### Compilation

```bash
make
```

This produces the `ircserv` executable. To clean up:

```bash
make clean    # removes object files
make fclean   # removes object files and binary
make re       # full rebuild
```

### Running the server

```bash
./ircserv <port> <password>
```

- `port` — the TCP port the server listens on (e.g. `6667`)
- `password` — required for any client wishing to connect

Example:

```bash
./ircserv 6667 pass123
```

---

## Usage

### Connecting with an IRC client

Any standard IRC client can be used. Recommended reference clients:

- **WeeChat** — `weechat` (terminal)
- **HexChat** — graphical client
- **irssi** — lightweight terminal client

Example WeeChat connection:

```
/server add local 127.0.0.1/6667 -password=pass123
/connect local
```

### Quick test with netcat

You can manually send IRC messages to verify server behavior:

```bash
nc -C 127.0.0.1 6667
```

Then type (or paste) a sequence of IRC commands:

```
PASS pass123
NICK mynick
USER myuser 0 * :My Real Name
JOIN #42
PRIVMSG #42 :Dzień dobry, world!
QUIT :bye
```

To test partial message handling (packet fragmentation), use `ctrl+D` in `nc` to flush chunks:

```
com^Dman^Dd
```

---

## Resources

### IRC Protocol References

- [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812 — IRC: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [RFC 2813 — IRC: Server Protocol](https://datatracker.ietf.org/doc/html/rfc2813)
- [Modern IRC Client Protocol (ircv3.net)](https://modern.ircdocs.horse/)
- [IRC Numerics Reference](https://www.alien.net.au/irc/irc2numerics.html)

### Networking / epoll References

- [epoll(7) Linux man page](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [The Linux Programming Interface — Michael Kerrisk](https://man7.org/tlpi/)

### AI Usage

AI tools were used during this project for the following purposes:

- **Understanding protocol details** — querying nuances of RFC 1459/2812 (e.g. MODE flag interactions, registration flow edge cases)
- **Debugging assistance** — describing observed behavior and asking for possible causes
- **Documentation** — drafting and structuring this README

All AI-generated content was reviewed, tested, and validated by the team before being incorporated. No production code was copied directly from AI output without full understanding and manual verification.
