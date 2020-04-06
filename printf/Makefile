SOURCES = printf.asm
OBJECTS = $(SOURCES:.asm=.o)

all: $(SOURCES) obj exe del run

obj:
	nasm -f elf64 -o $(OBJECTS) $(SOURCES)

exe:
	ld -s -o code $(OBJECTS)
run:
	./code
del:
	rm -rf *.o
