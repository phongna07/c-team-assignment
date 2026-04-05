CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
TARGET = gpa_manager
OBJS = main.o gui.o logic.o file_manager.o

# GTK4 Flags
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS = $(shell pkg-config --libs gtk4)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(GTK_LIBS)

gui: $(TARGET)

main.o: main.c gui.h logic.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c main.c

gui.o: gui.c gui.h file_manager.h logic.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c gui.c

logic.o: logic.c logic.h
	$(CC) $(CFLAGS) -c logic.c

file_manager.o: file_manager.c file_manager.h logic.h
	$(CC) $(CFLAGS) -c file_manager.c

run: all
	./$(TARGET)

run-gui: gui
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
