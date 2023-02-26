/****************************************************************
 * Developer:	Celso Duran										*
 * Client:		Dataremote Inc.									*
 * Date:		02/24/2023										*
 * Project: Embedded Team Test - Base64 UDP Server				*
 ***************************************************************/

#include "sockets.h"
#include "misc.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct sockaddr_in my_sock, their_sock;
socklen_t my_addr_size = sizeof(my_sock);
socklen_t their_addr_size = sizeof(their_sock);

int socketId;

int initialize_socket(unsigned short port_num)
{
	socketId = 0;

	if(!(socketId = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		errout("Socket error");
	}

	memset(&my_sock, '\0', my_addr_size);
 	my_sock.sin_family = AF_INET;
 	my_sock.sin_port = htons(port_num);
 	my_sock.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

 	// A bind is first attempted with the given port number
 	// If the bind fails the code binds with port 0 which then will
 	// bind with the first available port and print the port to the terminal for the user
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

 	return socketId;
 }


size_t receive_data(void *input_buffer, size_t len)
{
	size_t chars_read = 0;
	if ((chars_read = recvfrom(socketId, input_buffer, len, 0, (struct sockaddr*)&their_sock, &their_addr_size)) < 0)
 	{
        	errout("Receive error occured");
    }
	return chars_read;
}

void send_data(void *output_buffer, size_t len)
{
	sendto(socketId, output_buffer, len, 0, (struct sockaddr*)&their_sock, their_addr_size);
}
