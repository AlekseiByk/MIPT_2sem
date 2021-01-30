HEADERS = headers/ASSert.h headers/Text.h headers/functions.h headers/Header.h

all:lang remove

lang: lang.o func.o ASM.o Language.o Print.o
	g++ $^ -o lang

func.o: $(HEADERS)
	g++ -c headers/func.cpp -o $@


lang.o: lang.cpp $(HEADERS)
	g++ -c $< -o $@

ASM.o: $(HEADERS)
	g++ -c headers/ASM.cpp -o $@

Language.o: $(HEADERS)
	g++ -c headers/Language.cpp -o $@

Print.o: $(HEADERS)
	g++ -c headers/Print.cpp -o $@

remove:
	rm -rf lang.o
	rm -rf func.o
	rm -rf ASM.o
	rm -rf Language.o
	rm -rf Print.o
	

toasm:
	temp/./assembler
	temp/./processor


