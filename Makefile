CC=gcc
RM=rm
CFLAGS=-I. -Wall -ggdb -Wunused-function
TARGET=list
LIBS = -lpthread

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
INC=$(wildcard *.h)

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	@$(RM) $(TARGET)
