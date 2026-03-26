CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
TARGET = gpa_manager
OBJS = main.o logic.o file_manager.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c logic.h file_manager.h
	$(CC) $(CFLAGS) -c main.c

logic.o: logic.c logic.h
	$(CC) $(CFLAGS) -c logic.c

file_manager.o: file_manager.c file_manager.h logic.h
	$(CC) $(CFLAGS) -c file_manager.c

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
