CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -I src -std=c++98

NAME = ircserv
SRC = 	src/main.cpp \
		src/Server.cpp \
		src/Client.cpp \
		src/Channel.cpp \
		src/Command.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
