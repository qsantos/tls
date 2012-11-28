CFLAGS  = -Wall -Werror -pedantic -std=c99 -O3
LDFLAGS = -O3
EXEC    = tls
SRC     = $(wildcard *.c)
OBJ     = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	gcc $(LDFLAGS) $(OBJ) -o $(EXEC)

%.o: %.c *.h
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJ)
