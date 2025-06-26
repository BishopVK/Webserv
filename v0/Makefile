NAME	=	webserv

COMPILER	=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98 -g
RM	=	rm -rf

SRCS	=	./src/main.cpp \
			./src/server/HttpServer.cpp \
			./src/io/SocketUtils.cpp \
			./src/io/Multiplexer.cpp \
			./src/http/HttpRequest.cpp \
			./src/http/HttpResponse.cpp \
			./src/http/HttpRequestHandler.cpp \
			./src/controllers/base/AController.cpp \
			./src/controllers/ControllerRegistry.cpp \
			./src/controllers/TaskController.cpp \
			./src/controllers/UserController.cpp \


OBJS    := $(SRCS:.cpp=.o)

%.o: %.cpp
	$(COMPILER) $(FLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	$(COMPILER) $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
.DEFAULT_GOAL = all
