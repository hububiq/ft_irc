CC = c++
NAME = irc
FLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I${INC_DIR} -I${INC_DIR}/domain -I${INC_DIR}/server -I${INC_DIR}/server/executor -I${INC_DIR}/server/executor/commands -I${INC_DIR}/server/parser -I${INC_DIR}/utils
SRCS = \
       src/main.cpp \
			 src/server/server_runner.cpp \
       src/domain/Server.cpp \
       src/server/multiplexer.cpp \
       src/server/listener.cpp \
       src/server/epoll_state_manager.cpp \
       src/server/server_runner.cpp \
       src/server/conn_handler.cpp \
       src/server/request_handler.cpp \
       src/server/executor/executor.cpp \
       src/server/parser/parser.cpp \
       src/domain/Channel.cpp \
       src/domain/Client.cpp \
       src/commands/command_handler.cpp \
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
       src/commands/User.cpp \
       src/utils/reply_factory.cpp
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
# 	find -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format --style=file:.clang-format -i

clean:
	${RM} ${OBJ_DIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re