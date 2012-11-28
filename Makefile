CFLAGS  = -Wall -Wextra -Werror -ansi -pedantic -std=c99 -O3
LDFLAGS = -O3
TARGET  = tls
SRC     = main.c network.c tls.c
OBJ     = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc $(LDFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

destroy: clean
	rm -f $(TARGET)

rebuild: destroy all
