CC = c++
NAME = irc
FLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
INC_DIR = include
COMM_INC_DIR = include/commands
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I${INC_DIR} -I${COMM_INC_DIR}
SRCS = src/main.cpp \
       src/Channel.cpp \
       src/Client.cpp \
       src/Parser.cpp \
       src/Replies.cpp \
       src/Server.cpp \
       src/commands/CommandHandler.cpp \
       src/commands/Cap.cpp \
       src/commands/Invite.cpp \
       src/commands/Join.cpp \
       src/commands/Kick.cpp \
       src/commands/Mode.cpp \
       src/commands/Nick.cpp \
       src/commands/Pass.cpp \
       src/commands/Ping.cpp \
       src/commands/PrivMsg.cpp \
       src/commands/Quit.cpp \
       src/commands/Topic.cpp \
       src/commands/User.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
RM = rm -rf

all: ${NAME}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} ${FLAGS} ${INCLUDES} -c $< -o $@

${NAME}: ${OBJS}
	${CC} ${FLAGS} $^ -o $@

format:
	find -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format --style=Google -i

clean:
	${RM} ${OBJ_DIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
