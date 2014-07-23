EXEC=ssdie 
OBJECTS=ssdie.o sql_functions.o file_functions.o
CFLAGS= -c -g -Wall 
CC= c99
LIB=-lsqlite3

all: $(EXEC)
	make clean

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIB)

ssdie.o: 
	$(CC) $(CFLAGS) src/ssdie.c

sql_functions.o:
	$(CC) $(CFLAGS) src/sql_functions.c

file_functions.o:
	$(CC) $(CFLAGS) src/file_functions.c
clean:
	rm -f *.o
