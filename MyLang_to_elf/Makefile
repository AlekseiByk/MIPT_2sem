HEADERS = headers/ASSert.h headers/Text.h headers/functions.h headers/Header.h
OBJS = lang.o func.o ASM.o Language.o Print.o

all:lang clean

lang: $(OBJS)
	g++ $^ -o $@

%.o: headers/%.cpp $(HEADERS) 
	g++ -c $< -o $@


clean:
	rm -rf *.o

toasm:
	temp/./assembler
	temp/./processor


