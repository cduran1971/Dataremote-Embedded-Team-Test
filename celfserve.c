#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Make sure the argument is a valid port number
_Bool is_port_number(char *a_string) {
	int length = strlen(a_string);

	// Port numbers have at most 5 digits
	if(length > 6)
		return false;

	for(int i=0; i < length; ++i) {
		if(a_string[i] < '0' || a_string[i] > '9' )
			return false;
	}
	return true;
}

int main(int argc, char *argv[]) {
	unsigned short port_num = 0;
	
	if((argc != 2) || (!is_port_number(argv[1]))) {
		printf("Invalid argument(s)\n");
		exit(0);
	}

	port_num = atoi(argv[1]);
	printf("Celfserve listening on port: %d\n", atoi(argv[1]));
}