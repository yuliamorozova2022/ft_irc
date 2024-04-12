
NAME =	ircserv
SRCS =	main.cpp Server.cpp Channel.cpp Client.cpp PollManager.cpp \
		tools.cpp commands.cpp replies.cpp mode_command.cpp

OBJS_DIR = ./obj/
OBJS = ${addprefix ${OBJS_DIR}, ${SRCS:.cpp=.o}}

CPP_FLAGS = --std=c++98 -Wall -Wextra -Werror -fsanitize=address -g
CPP = c++

# Colors
	NRM_COLOR = \033[0;39m
	GREEN = \033[0;92m

# NOTE: -w flag to hide warnings
all: ${NAME}

${OBJS_DIR}%.o : %.cpp
	@ mkdir -p ${OBJS_DIR}
	@ ${CPP} ${CPP_FLAGS} -c $< -o $@

${NAME}:	${OBJS}
	@ ${CPP} ${CPP_FLAGS} ${OBJS} -o ${NAME}
	@ echo "${GREEN}${NAME} created!${NRM_COLOR}"

clean:
	rm -rf ${OBJS_DIR}

fclean: clean
	rm -rf ${NAME}

re: fclean ${NAME}

.PHONY: all re clean fclean