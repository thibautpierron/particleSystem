# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpierron <tpierron@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/10 11:12:05 by tpierron          #+#    #+#              #
#    Updated: 2017/07/20 14:20:59 by tpierron         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


.PHONY: all clean fclean re

NAME = ParticleSystem

CC = clang++
SRC = classes/OpenCl.class.cpp classes/OpenGl.class.cpp classes/Shader.class.cpp math3d.cpp main.cpp sdl_tools.cpp

CFLAGS = -Wall -Wextra -Werror
OBJ_PATH = ./obj/
OBJ_NAME = $(SRC:.cpp=.o)

SDL = -I/Users/tpierron/.brew/include/SDL2 -L/Users/tpierron/.brew/lib -lSDL2
SDL_PATH = -I/Users/tpierron/.brew/include/SDL2
OPENGL = -framework OpenGl -framework AppKit
OPENCL = -framework OpenCL

OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))

all: $(NAME)
$(OBJ_PATH)%.o: %.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || echo "" > /dev/null
	@mkdir $(OBJ_PATH)/classes 2> /dev/null || echo "" > /dev/null
	$(CC) $(CFLAGS) -o $@ -c -Iclasses $(SDL_PATH) $< -std=c++11

$(NAME): $(OBJ)
	$(CC) $(SDL) $(OPENGL) $(OPENCL) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all
