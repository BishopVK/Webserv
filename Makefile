# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/20 12:20:33 by danjimen,is       #+#    #+#              #
#    Updated: 2025/06/26 12:12:40 by danjimen,is      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

SRC			=	main.cpp \
				src/server/Config.cpp \
				src/server/Location.cpp \
				src/server/Server.cpp \
				src/controllers/ControllerRegistry.cpp \
				src/controllers/TaskController.cpp \
				src/controllers/UserController.cpp \
				src/controllers/base/AController.cpp \
				src/http/HttpRequest.cpp \
				src/http/HttpRequestHandler.cpp \
				src/http/HttpResponse.cpp \
				src/http/HttpServer.cpp \
				src/io/Multiplexer.cpp \
				src/io/SocketUtils.cpp \
				src/valueObjects/IntValue.cpp \
				src/valueObjects/Port.cpp \

OBJ			=	$(SRC:.cpp=.o)

$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

rc:	re
	rm -f $(OBJ)

run:
	./$(NAME) $(ARG)

valgrind:
	valgrind --leak-check=full -s ./$(NAME) $(ARG)

.PHONY: all clean fclean re rc run valgrind
