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
#include "encode64.h"
#include "constants.h"


void errout(char *err_string) 
{
	printf("%s\n", err_string);
	exit(EXIT_FAILURE);
}

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
  	/************** Validate Arguments **************/
	if((argc != 2) || (!is_port_number(argv[1])))
	{
		errout("Invalid argument(s)");
	}
	unsigned short port_num = atoi(argv[1]);
	/************ End Validate Arguments ************/

	/***************** Socket Setup *****************/
	int socketId = 0;
	struct sockaddr_in my_sock, their_sock;
  	socklen_t my_addr_size, their_addr_size;

	if(!(socketId = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		errout("Socket error");
	}

	my_addr_size = sizeof(my_sock);
	memset(&my_sock, '\0', my_addr_size);
 	my_sock.sin_family = AF_INET;
 	my_sock.sin_port = htons(port_num);
 	my_sock.sin_addr.s_addr = inet_addr(INET_ADDR);

	int bindTries = MAX_BIND_RETRIES;
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
 	/*************** End Socket Setup ***************/

  	// ----------------------------------
  	size_t current_ob_size = 2*INPUT_BUFFER_SIZE;
  	char input_buffer[INPUT_BUFFER_SIZE];
  	unsigned char output_buffer[current_ob_size];
  	size_t ob_index = 0;
  	size_t ib_index = 0;
  	size_t chars_read = 0;
  	// ----------------------------------

	memset(output_buffer, '\0', current_ob_size*sizeof(char));
 	while(1) {
 		if ((chars_read = recvfrom(socketId, input_buffer, sizeof(input_buffer), 0, (struct sockaddr*)&their_sock, &their_addr_size)) < 0)
 		{
        	errout("Receive error occured");
    	}

    	for(ib_index=0; ib_index < chars_read; ib_index++) {
    		if(input_buffer[ib_index] == TERMINATOR_CHAR)
    		{
    			size_t input_size = ob_index;
    			size_t encoded_length = 0;
    			char *encoded_data = base64_encode(output_buffer, input_size, &encoded_length);

    			sendto(socketId, encoded_data, encoded_length, 0, (struct sockaddr*)&their_sock, their_addr_size);
    			sendto(socketId, "\n", 1, 0, (struct sockaddr*)&their_sock, their_addr_size);
				memset(output_buffer, '\0', current_ob_size*sizeof(char));
				free(encoded_data);
				ob_index = 0;
    			break;
    		}

    		output_buffer[ob_index] = (unsigned char)input_buffer[ib_index];
    		ob_index++;
    	}
 	}
 	
 	return 0;
}
