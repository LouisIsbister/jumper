CC = gcc # Compiler

CFLAGS = -Wall # compiler flags

TARGET = run # output executable name

# source files
SRCS = src\jumper.c src\commands.c src\utils.c

# build target
# $(TARGET): $(SRCS)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# dbg target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -g -o $(TARGET) $(SRCS)

clean:
	del .\$(TARGET).exe

	