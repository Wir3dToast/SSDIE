EXEC=ssdie 
OBJECTS=ssdie.o sql_functions.o
CFLAGS= -c -g -Wall 
CC= c99
LIB=-lsqlite3

all: $(EXEC)
	make clean

$(EXEC): ssdie.o sql_functions.o
	$(CC) $(OBJECTS) -o $(EXEC) $(LIB)

ssdie.o: 
	$(CC) $(CFLAGS) src/ssdie.c

sql_functions.o:
	$(CC) $(CFLAGS) src/sql_functions.c
clean:
	rm -f *.o
