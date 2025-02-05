CC= gcc
CFLAGS= -Werror -pthread
EXE= server
SOURCES= \
	src/main.c \
	src/str.c \
	src/socket.c \
	src/util.c \
	src/queue.c \
	src/http_endpoints.c \
	src/hashmap.c \
	src/endpoint_handlers.c

PORT= 3333

$(EXE): clean 
	$(CC) $(SOURCES) $(CFLAGS) -o $@

memory: $(EXE)
	leaks --atExit -- ./$(EXE) $(PORT)

clean: 
	rm -f *.o $(EXE)

