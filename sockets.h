/****************************************************************
 * Developer:	Celso Duran										*
 * Client:		Dataremote Inc.									*
 * Date:		02/24/2023										*
 * Project: Embedded Team Test - Base64 UDP Server				*
 ***************************************************************/

#include<arpa/inet.h>
#include<sys/socket.h>

extern struct sockaddr_in my_sock, their_sock;
extern socklen_t my_addr_size;
extern socklen_t their_addr_size;

int initialize_socket(unsigned short port_num);
size_t receive_data(void *input_buffer, size_t len);
void send_data(void *output_buffer, size_t len);