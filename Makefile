PROG = Chess
CC = g++
FLAGS = -std=c++17
OBJS = Main.o Pieces.o Foundation.o UnitTest.o

$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) 

Main.o : Main.cpp 
	$(CC) $(FLAGS) -c Main.cpp

Pieces.o : Pieces.cpp 
	$(CC) $(FLAGS) -c Pieces.cpp

Foundation.o : Foundation.cpp 
	$(CC) $(FLAGS) -c Foundation.cpp

UnitTest.o : UnitTest.cpp 
	$(CC) $(FLAGS) -c UnitTest.cpp

clean:
	rm $(PROG) $(OBJS)

