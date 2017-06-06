# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amathias <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/03 11:09:29 by amathias          #+#    #+#              #
#    Updated: 2016/09/15 10:30:20 by amathias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INC = /

SRC =	src/main.cpp			\
		src/env.cpp				\
		src/CLenv.cpp			\
		src/Shader.cpp			\
		src/Matrix.cpp			\
		src/Vec3.cpp			\
		src/Camera.cpp			\
		src/InputHandler.cpp	\
		src/Scene.cpp			\

SRC_C = third-party/glad/glad.c	\

OBJ = $(SRC:.cpp=.o)
OBJ_C = $(SRC_C:.c=.o)

NAME = particle-system
CC = clang
RM = rm -f
CFLAGS = -std=gnu++14 -Wall -Werror -Wextra

LIB_PATH = ./third-party/glad/	./third-party/glfw/src/
LIB_NAME = -framework OpenCL -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

LIB = $(addprefix -L,$(LIB_PATH))
LFLAGS = $(LIB) $(LIB_NAME)

INC_PATH = ./third-party/ ./third-party/glad/ ./third-party/glfw/include/
INC = $(addprefix -I,$(INC_PATH))

all: $(NAME)

$(NAME): $(OBJ_C) $(OBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) $(OBJ_C) -o $@

%.o: %.cpp
	$(CC) $(INC) -o $@ -c $^ $(CFLAGS)

%.o: %.c
	$(CC) $(INC) -o $@ -c $^

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
