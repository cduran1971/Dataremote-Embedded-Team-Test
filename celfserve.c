// Task:
// Create a UDP server using standard libc calls and Linux APIs.
// The server application must take as only parameter the port number it will listen to.
// The server will accumulate the information sent by the client.
// If the server detects a <CR> caracter (Decimal 13, Hex 0x0D), the server will send to client the Base 64 representation of the data received so far.
// The server forgets all data before <CR> and starts over.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

const unsigned short kMAX_PORT_NUMBER = 65535;

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

	if(atoi(a_string) > kMAX_PORT_NUMBER)
		return false;

	return true;
}

// If a port number of 0 is given the server will bind to the
// first open port and display the port used
int main(int argc, char *argv[]) {
	unsigned short port_num = 0;
	
	if((argc != 2) || (!is_port_number(argv[1]))) {
		printf("Invalid argument(s)\n");
		exit(EXIT_FAILURE);
	}

	port_num = atoi(argv[1]);
	printf("Celfserve listening on port: %d\n", atoi(argv[1]));
}