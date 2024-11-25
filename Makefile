CC = gcc
CFLAGS = -Wall -Wextra -Werror
SOURCES = main.c wordle.c player.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = main

.PHONY: all clean run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	@./$(EXECUTABLE)

clean:
	@rm -f $(OBJECTS) $(EXECUTABLE)
