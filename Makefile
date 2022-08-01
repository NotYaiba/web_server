SRCS =  main.cpp \
		Parser.cpp \
		Server.cpp \
		Location.cpp \
		Socket.cpp \
		Connection.cpp \
		WebServer.cpp \
		./http/Request.cpp\
		./http/Response.cpp\
		./utils/Tools.cpp \
		Cgi.cpp\

NAME = webserv

all : $(NAME)

FLAGS =  -std=c++98  -Wall -Wextra -Werror

$(NAME) : $(SRCS)
		mkdir -p .tmp tmp
		@clang++ ${FLAGS} ${SRCS}   -o $(NAME)


sani : $(SRCS)
		@clang++ ${FLAGS} ${SRCS} -fsanitize=address -g -o $(NAME)
clean :
		@rm -f ${NAME}
		
fclean : clean
		@rm -f $(NAME)
		rm  -rf tmp
		rm -rf .tmp 

re : fclean
	rm -f body_debug 
	make all
