deps = node.c linked_list.c map.c string_vector.c list.c src/server.c


compile_server:
	gcc -Wall assignment3.c $(deps)  -o out/server.o -pthread -std=c99
