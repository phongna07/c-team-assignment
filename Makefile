CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic
TARGET = gpa_manager
GUI_TARGET = gpa_manager_gui
OBJS = main.o logic.o file_manager.o

# GTK4 Flags
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS = $(shell pkg-config --libs gtk4)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

gui: logic.o file_manager.o
	$(CC) $(CFLAGS) -Wno-deprecated-declarations $(GTK_CFLAGS) gui.c logic.o file_manager.o -o $(GUI_TARGET) $(GTK_LIBS)

main.o: main.c logic.h file_manager.h
	$(CC) $(CFLAGS) -c main.c

logic.o: logic.c logic.h
	$(CC) $(CFLAGS) -c logic.c

file_manager.o: file_manager.c file_manager.h logic.h
	$(CC) $(CFLAGS) -c file_manager.c

run: all
	./$(TARGET)

run-gui: gui
	./$(GUI_TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(GUI_TARGET)
