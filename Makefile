SRCS =  main.cpp \
		Parser.cpp \
		Server.cpp \
		Location.cpp \
		Socket.cpp \
		Connection.cpp \
		WebServer.cpp \
		Request.cpp\
		./utils/Tools.cpp \

NAME = webserv

all : $(NAME)

FLAGS =  -std=c++98 #-Wall -Wextra -Werror

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
		rm  -f tmp/* 
		rm -f body_debug 
