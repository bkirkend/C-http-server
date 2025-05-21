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
	src/endpoint_handlers.c \
	src/endpoint_parse.c

PORT=80

$(EXE): clean 
	$(CC) $(SOURCES) $(CFLAGS) -o $@ && ./$(EXE) $(PORT)

run : $(EXE)
	./$(EXE) $(PORT)

memory: $(EXE)
	leaks --atExit -- ./$(EXE) $(PORT)

clean: 
	rm -f *.o $(EXE)

