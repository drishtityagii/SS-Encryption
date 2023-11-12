CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wstrict-prototypes
LDFLAGS = $(shell pkg-config --libs gmp)
EXEC = keygen encrypt decrypt
OBJS = keygen.o encrypt.o decrypt.o randstate.o numtheory.o ss.o

all: $(EXEC)
keygen: keygen.o randstate.o numtheory.o ss.o
		$(CC) -o keygen keygen.o randstate.o numtheory.o ss.o $(LDFLAGS)
encrypt: encrypt.o randstate.o numtheory.o ss.o 
		$(CC) -o encrypt encrypt.o randstate.o numtheory.o ss.o $(LDFLAGS)
decrypt: decrypt.o randstate.o numtheory.o ss.o
		$(CC) -o decrypt decrypt.o randstate.o numtheory.o ss.o $(LDFLAGS)
keygen.o: keygen.c
		$(CC) $(CFLAGS) -c keygen.c
encrypt.o: encrypt.c
		$(CC) $(CFLAGS) -c encrypt.c
decrypt.o: decrypt.c
		$(CC) $(CFLAGS) -c decrypt.c
numtheory.o: numtheory.c
		$(CC) $(CFLAGS) -c numtheory.c
randstate.o: randstate.c
		$(CC) $(CFLAGS) -c randstate.c
ss.o: ss.c
		$(CC) $(CFLAGS) -c ss.c
pkg-config:
		sudo apt install pkg-config libgmp-dev
clean:
		rm -f keygen encrypt decrypt *.o
format:
		clang-format -i -style=file *.[ch]
