CC = gcc
CFLAGS = -Wall
TARGET = a2

SRCFILES = main.c mergesort.c utils.c
OBJFILES = $(SRCFILES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(OBJFILES) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJFILES) $(TARGET) *~