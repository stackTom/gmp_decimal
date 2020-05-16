CC = gcc
CFLAGS = -g
DEPS =
OBJ = gmp_decimal.o
LIBS = -lgmp

all: gmp_decimal

gmp_decimal.o: gmp_decimal.c $(DEPS)
	$(CC) -c -o gmp_decimal.o $< $(CFLAGS) $(LIBS)

gmp_decimal: main.c $(OBJ)
	$(CC) -o gmp_decimal $^ $(CFLAGS) $(LIBS)

valgrind: all
	valgrind --leak-check=full ./gmp_decimal

run: all
	./gmp_decimal

clean:
	rm -rf *.o
	rm -rf gmp_decimal

