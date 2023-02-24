celfserve: celfserve.c encode64.c misc.c sockets.c
	clang -std=c17 -o celfserve celfserve.c encode64.c misc.c sockets.c -I.