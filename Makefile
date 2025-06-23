CC = gcc # Compiler

CFLAGS = -g -pedantic # compiler flags

TARGET = run # output executable name

# source files
SRCS = src\jumper.c src\commands.c src\utils.c

# dbg target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS)  -o $(TARGET) $(SRCS)

clean:
	del .\$(TARGET).exe

	