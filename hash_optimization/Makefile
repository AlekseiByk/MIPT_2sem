al:
	gcc  -O0 -g Hash_table.cpp func_1.s -o hash
call:
	valgrind --tool=callgrind ./hash
time:
	time ./hash
strcmp:
	gcc -Wa,-adhln -g -O0 Hash_table.cpp func_1.s > assembly_list.s
	g++ -S -O0 Hash_table.cpp func_1.s

