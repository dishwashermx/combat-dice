NAME = dice
SRCDIR = src
INCDIR = incl
OBJDIR = obj

# Core source files (stays same)
SRC = main.cpp Game.cpp Die.cpp Display.cpp Input.cpp CharacterFactory.cpp Character.cpp Wave.cpp

# Build rules
SRCFILES = $(addprefix $(SRCDIR)/, $(SRC))
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

FLAGS = -Wall -Wextra -Werror
INCLUDES = -I$(INCDIR)

all: $(OBJDIR) $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJ)
	g++ $(FLAGS) -o $(NAME) $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(NAME) $(OBJ)

fclean: clean
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re