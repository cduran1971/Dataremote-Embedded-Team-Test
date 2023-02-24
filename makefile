celfserve: celfserve.c encode64.c
	clang -std=c17 -o celfserve celfserve.c encode64.c -I.