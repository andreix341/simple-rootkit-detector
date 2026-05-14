CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude -g
SRC     = main.c src/proc_scan.c src/kill_scan.c src/dirent_scan.c src/compare.c
TARGET  = rootkit-detector

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
