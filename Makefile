EXEC=ssdie 
OBJECTS=main.o ssdie.o sql_functions.o file_functions.o auxiliary.o 
CFLAGS= -c -g -Wall 
CC= c99
LIB=-lsqlite3

all: $(EXEC)
	make clean

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIB)
main.o:
	$(CC) $(CFLAGS) src/main.c
ssdie.o: 
	$(CC) $(CFLAGS) src/ssdie.c

sql_functions.o:
	$(CC) $(CFLAGS) src/sql_functions.c

file_functions.o:
	$(CC) $(CFLAGS) src/file_functions.c
auxiliary.o:
	$(CC) $(CFLAGS) src/auxiliary.c
clean:
	rm -f *.o
