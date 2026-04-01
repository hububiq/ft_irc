CC = c++
NAME = irc
FLAGS = -Wall -Wextra -Werror -std=c++98
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
INCLUDES = -I${INC_DIR}
SRCS = ${wildcard ${SRC_DIR}/*.cpp}
OBJS = ${SRCS:${SRC_DIR}/%.cpp=${OBJ_DIR}/%.o}
RM = rm -rf

all: ${NAME}

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	${RM} ${OBJ_DIR}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY all clean fclean re