all:server.c client.c
	gcc -o server server.c -lpthread
	gcc -o client client.c -lpthread
	gcc -o clientinput clientinput.c -lpthread
	chmod 755 run_tests
	./server
