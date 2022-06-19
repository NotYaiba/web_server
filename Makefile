SRCS =  main.cpp \
		Parser.cpp \
		Server.cpp \
		Location.cpp \
		Socket.cpp \

NAME = webserv

all : $(NAME)

FLAGS = -Wall -Wextra -Werror -std=c++98

$(NAME) : $(SRCS)
		@clang++ ${FLAGS} ${SRCS}   -o $(NAME)


sani : $(SRCS)
		@clang++ ${FLAGS} ${SRCS} -fsanitize=address -g -o $(NAME)
clean :
		@rm -f ${NAME}

fclean : clean
		@rm -f $(NAME)

re : fclean
		make all
