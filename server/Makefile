# Only the next 3 lines need to be edited - leave the rest!
TARGET = server2
DEPS = rdwrn.h
OBJ = rdwrn.o server2.o

CC=cc
CFLAGS=-I. -Wall -g -O0 -pthread

all: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)




