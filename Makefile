NAME = dice
SRC = main.cpp Game.cpp Die.cpp Display.cpp Input.cpp
OBJ = $(SRC:.cpp=.o)
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	g++ $(FLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	g++ $(FLAGS) -c $< -o $@

clean:
	rm -f $(NAME) $(OBJ)

re: clean all

.PHONY: all clean re