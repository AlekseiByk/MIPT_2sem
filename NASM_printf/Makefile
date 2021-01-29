SOURCES = printf_jumptable.s
OBJECTS = $(SOURCES:.s=.o)

all: $(SOURCES) obj exe del run

obj:
	nasm -f elf64 -o $(OBJECTS) $(SOURCES)

exe:
	ld -s -o code $(OBJECTS)
run:
	./code
del:
	rm -rf *.o
