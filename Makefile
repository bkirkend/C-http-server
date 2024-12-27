CC= gcc
CFLAGS= -Werror
EXE= server
SOURCES= \
	src/main.c \
	src/str.c \
	src/socket.c \
	src/util.c \
	src/http_endpoints.c
PORT= 3333

$(EXE): clean 
	$(CC) $(SOURCES) $(CFLAGS) -o $@

memory: $(EXE)
	leaks --atExit -- ./$(EXE) $(PORT)

clean: 
	rm -f *.o $(EXE)

