CC = c++
NAME = irc
FLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I${INC_DIR} -I${INC_DIR}/domain -I${INC_DIR}/server -I${INC_DIR}/server/executor -I${INC_DIR}/server/executor/commands -I${INC_DIR}/server/parser -I${INC_DIR}/utils
SRCS = \
       src/main.cpp \
       src/server/ServerRunner.cpp \
       src/domain/ServerDao.cpp \
       src/server/Multiplexer.cpp \
       src/server/Listener.cpp \
       src/server/EpollStateManager.cpp \
       src/server/EpollInitializer.cpp \
       src/server/ConnHandler.cpp \
       src/utils/reply_factory.cpp \
       src/server/RequestHandler.cpp \
       src/server/executor/Executor.cpp \
       src/server/executor/CommandHandler.cpp \
       src/server/executor/commands/Cap.cpp \
       src/server/executor/commands/Invite.cpp \
       src/server/executor/commands/Join.cpp \
       src/server/executor/commands/Kick.cpp \
       src/server/executor/commands/Mode.cpp \
       src/server/executor/commands/Nick.cpp \
       src/server/executor/commands/Pass.cpp \
       src/server/executor/commands/Ping.cpp \
       src/server/executor/commands/PrivMsg.cpp \
       src/server/executor/commands/Quit.cpp \
       src/server/executor/commands/Topic.cpp \
       src/server/executor/commands/User.cpp \
       src/server/executor/JoinGatekeeper.cpp \
       src/server/executor/MessageParser.cpp \
       src/server/executor/ModeReporter.cpp \
       src/server/executor/Validator.cpp \
       src/domain/Channel.cpp \
       src/domain/Client.cpp
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