CC = gcc
CFLAGS = -Wall -Wextra -I./Include
SOURCES = main.c Source/Richiesta.c Source/Tecnico.c Source/Utility.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = sistema_interventi

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
