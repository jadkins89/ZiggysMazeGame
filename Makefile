CXX = g++
CXXFLAGS	= -std=c++11 -Wall

all: maze

clean:
	rm Player.o Board.o  Maze.o maze

maze: Player.o Board.o Maze.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp Player.o Board.o Maze.o -o maze

Player.o: Player.cpp
	$(CXX) $(CXXFLAGS) -c Player.cpp

Board.o: Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp

Maze.o: Maze.cpp
	$(CXX) $(CXXFLAGS) -c Maze.cpp
