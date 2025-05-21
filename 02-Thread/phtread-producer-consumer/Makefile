CC=gcc
CFLAGS=-pthread -Wall
SRC=src/main.c src/buffer.c 
OBJ=$(SRC:.c=.o)
TARGET=producer-consumer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c src/buffer.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)