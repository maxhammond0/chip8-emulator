CC = gcc
CFLAGS = -Wall -Wextra -Iinclude $(shell pkg-config --cflags sdl2) -Wc23-extensions
LDFLAGS = $(shell pkg-config --libs sdl2)

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = emulator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
