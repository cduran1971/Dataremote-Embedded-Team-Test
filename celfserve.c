// Task:
// Create a UDP server using standard libc calls and Linux APIs.
// The server application must take as only parameter the port number it will listen to.
// The server will accumulate the information sent by the client.
// If the server detects a <CR> caracter (Decimal 13, Hex 0x0D), the server will send to client the Base 64 representation of the data received so far.
// The server forgets all data before <CR> and starts over.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const unsigned short kMAX_PORT_NUMBER = 65535;
const char kINET_ADDR[] = "127.0.0.1";
const int kMAX_BIND_RETRIES = 3;
const int kinput_buffer_SIZE = 1024;

void errout(char *err_string) 
{
	printf("%s\n", err_string);
	exit(EXIT_FAILURE);
}

// Make sure the argument is a valid port number
int is_port_number(char *a_string)
{
	int length = strlen(a_string);

	// Port numbers have at most 5 digits
	if(length > 6)
		return 0;

	for(int i=0; i < length; ++i) {
		if(a_string[i] < '0' || a_string[i] > '9' )
			return 0;
	}

	if(atoi(a_string) > kMAX_PORT_NUMBER)
		return 0;

	return 1;
}

int main(int argc, char *argv[])
{
	unsigned short port_num = 0;
	int bindTries = kMAX_BIND_RETRIES;
	int socketId;
  	struct sockaddr_in my_sock, their_sock;
  	char input_buffer[kinput_buffer_SIZE];
  	socklen_t my_addr_size, their_addr_size;

  	// ----------------------------------
  	char output_buffer[10240];
  	int output_buffer_index = 0;
  	int input_buffer_index = 0;
  	int num_chars_in;
  	// ----------------------------------

	if((argc != 2) || (!is_port_number(argv[1])))
	{
		errout("Invalid argument(s)");
	}

	if(!(socketId = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		errout("Socket error");
	}
	
	port_num = atoi(argv[1]);

	my_addr_size = sizeof(my_sock);
	memset(&my_sock, '\0', my_addr_size);
 	my_sock.sin_family = AF_INET;
 	my_sock.sin_port = htons(port_num);
 	my_sock.sin_addr.s_addr = inet_addr(kINET_ADDR);

 	while(bindTries--)
 	{
 		if(bind(socketId, (struct sockaddr*)&my_sock, my_addr_size) == 0)
 		{
 			getsockname(socketId, (struct sockaddr*)&my_sock, &my_addr_size);
 			port_num = ntohs(my_sock.sin_port);
 			printf("Celfserve listening on port: %d\n", port_num);
 			break;
 		}

 		if(port_num == 0)
 		{
 			errout("Bind error occured");
 		} else {
 			printf("Port %d cannot be used, searching for next available port...\n", port_num);
 			port_num = 0;
 			my_sock.sin_port = htons(port_num);
 		}
 	}

 	their_addr_size = sizeof(their_sock);

 	while(1) {
 		if ((num_chars_in = recvfrom(socketId, input_buffer, sizeof(input_buffer), 0, (struct sockaddr*)&their_sock, &their_addr_size)) < 0)
 		{
        	errout("Receive error occured");
    	}

    	for(input_buffer_index=0; input_buffer_index < num_chars_in; input_buffer_index++) {
    		output_buffer[output_buffer_index] = input_buffer[input_buffer_index];
    		output_buffer_index++;
    	}

    	if(strchr(output_buffer, (char)13))
    	{
    		output_buffer[output_buffer_index] = 0;
			printf("%s\n", output_buffer);
			output_buffer_index = 0;	// Clear the output buffer
    	}
 	}
 	
 	return 0;
}

