PROG = Chess
CC = g++
FLAGS = -std=c++17
OBJS = Main.o Pieces.o Board.o Foundation.o Game.o

$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) 

Main.o : Main.cpp 
	$(CC) $(FLAGS) -c Main.cpp

Pieces.o : Pieces.cpp 
	$(CC) $(FLAGS) -c Pieces.cpp

Board.o : Board.cpp 
	$(CC) $(FLAGS) -c Board.cpp

Foundation.o : Foundation.cpp 
	$(CC) $(FLAGS) -c Foundation.cpp

Game.o : Game.cpp 
	$(CC) $(FLAGS) -c Game.cpp

clean:
	rm $(PROG) $(OBJS)

