EXEC=ssdie
OBJECTS=ssdie.o
SOURCES= ssdie.c 
CFLAGS= -c -g -Wall 
CC= c99
LIB=-lsqlite3

all: $(EXEC)
	make clean

$(EXEC): ssdie.o
	$(CC) $(OBJECTS) -o $(EXEC) $(LIB)

ssdie.o: 
	$(CC) $(CFLAGS) $(SOURCES)
clean:
	rm -f *.o
