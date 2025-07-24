# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/20 12:20:33 by danjimen,is       #+#    #+#              #
#    Updated: 2025/07/24 10:29:05 by danjimen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g

HEADERS		=	-I./include -I./include/valueObjects -I./include/utils -I./include/http -I./src/io

VPATH		=	src src/server src/controllers src/controllers/base src/http src/io src/valueObjects src/parse src/utils src/cgis .

SRC			=	main.cpp \
				Config.cpp \
				Location.cpp \
				Server.cpp \
				HttpRequest.cpp \
				HttpRequestHandler.cpp \
				HttpResponse.cpp \
				HttpServer.cpp \
				ClientConnection.cpp \
				ServerConnection.cpp \
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
				Logger.cpp \
				ContentTypeManager.cpp \
				FileSystemHandler.cpp \
				AutoIndexGenerator.cpp \
				LocationMatcher.cpp \
				PathHandler.cpp \
				Cgis.cpp \
				CgiHandler.cpp \
				AbstractHttpMethodHandler.cpp \
				HttpGetRequestHandler.cpp \
				HttpPostRequestHandler.cpp \
				HttpDeleteRequestHandler.cpp \
				ErrorPageGenerator.cpp \
				FileUploadHandler.cpp \
				File.cpp \

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

run: all
#	./$(NAME) $(ARG)
	./$(NAME) config.conf

valgrind:
	valgrind --leak-check=full -s ./$(NAME) $(ARG)

.PHONY: all clean fclean re rc run valgrind
