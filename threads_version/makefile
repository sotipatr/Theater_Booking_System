all:server_th.c client.c clientinput.c
	gcc -o server_th server_th.c -lpthread
	gcc -o client client.c -lpthread
	gcc -o clientinput clientinput.c -lpthread
	chmod 755 run_tests.sh
	./server_th
