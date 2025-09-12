NAME = dice
SRC = main.cpp
HEADERS = Die.hpp Hero.hpp
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(SRC) $(HEADERS)
		g++ $(FLAGS) -o $(NAME) $(SRC) $(HEADERS)

clean:
		rm -f $(NAME)

re: clean all

.PHONY: all clean re