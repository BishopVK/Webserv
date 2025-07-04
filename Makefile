# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/20 12:20:33 by danjimen,is       #+#    #+#              #
#    Updated: 2025/07/04 13:15:56 by danjimen,is      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

HEADERS		=	-I./include -I./include/valueObjects -I./src/http -I./src/io -I./src/controllers

VPATH		=	src src/server src/controllers src/controllers/base src/http src/io src/valueObjects src/parse .

SRC			=	main.cpp \
				Config.cpp \
				Location.cpp \
				Server.cpp \
				ControllerRegistry.cpp \
				TaskController.cpp \
				UserController.cpp \
				AController.cpp \
				HttpRequest.cpp \
				HttpRequestHandler.cpp \
				HttpResponse.cpp \
				HttpServer.cpp \
				Multiplexer.cpp \
				SocketUtils.cpp \
				IntValue.cpp \
				Port.cpp \
				Parser.cpp \
				parseConfig.cpp \
				parseUtils.cpp \
				parseServer.cpp \
				parseDirective.cpp \
				parseLocation.cpp \

# .o folder
OBJDIR		=	obj

# objects files (whit obj/ prefix)
OBJ			=	$(SRC:.cpp=.o)
OBJ			:= $(addprefix $(OBJDIR)/, $(OBJ))

# Main rule
$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(HEADERS) $(OBJ) -o $(NAME)

# Rule to create obj/ if doesn't exist and compile every .cpp
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(HEADERS) -c $< -o $@

# Create obj/ directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

all: $(NAME)

clean:
	rm -f $(OBJ)
	rm -rf $(OBJDIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

rc:	re
	rm -f $(OBJ)

run:
#	./$(NAME) $(ARG)
	./$(NAME) config2.conf

valgrind:
	valgrind --leak-check=full -s ./$(NAME) $(ARG)

.PHONY: all clean fclean re rc run valgrind
