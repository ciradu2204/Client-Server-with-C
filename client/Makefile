# Only the next 3 lines need to be edited - leave the rest!
TARGET = client2 
DEPS = rdwrn.h
OBJ = rdwrn.o client2.o

CC=cc
CFLAGS=-I. -Wall -g -O0

all: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)




