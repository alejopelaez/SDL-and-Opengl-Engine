CC = g++
CXXFLAGS = -g -Wall -Wextra -ansi
LINK = -lSDL -lSDL_image -lGL -lGLU
EXEC = SDL_Example

SRC = main.cpp Engine.cpp Input/Keyboard.cpp Input/Mouse.cpp Utility/Timer.cpp Utility/TextureLoader.cpp Utility/Font.cpp Errors/ErrorLog.cpp
OBJ = $(SRC:%.cpp=%.o)

all: EXEC

EXEC: $(OBJ)
	$(CC) $(LINK) -o $(EXEC) $^

clean:
	rm *.o */*.o make.depend


make.depend:	$(SRC)
	g++ -M $^ > $@

include make.depend
