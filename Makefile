# Compiler
CC = gcc

# Flags
CFLAGS = -I .\\include -L .\\lib  -lraylib -lwinmm -lgdi32 -lopengl32

# Source File
SRC = $(shell powershell -Command "(Get-ChildItem -Path src -Filter *.c | Sort-Object LastWriteTime -Descending | Select-Object -First 1).FullName")

# Output Files
OUT = exe/$(shell powershell -Command "[System.IO.Path]::GetFileNameWithoutExtension('$(SRC)')").exe

# Compile the File
all:
	$(CC) "$(SRC)" -o "$(OUT)" $(CFLAGS)

# Run The Compiled File
run: all
	./"$(OUT)"

# Clear the compiled File
clean:
	rm -f "$(OUT)"

