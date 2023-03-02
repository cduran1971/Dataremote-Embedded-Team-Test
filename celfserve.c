/****************************************************************
 * Developer:	Celso Duran										*
 * Client:		Dataremote Inc.									*
 * Date:		02/24/2023										*
 * Project: Embedded Team Test - Base64 UDP Server				*
 * Description:													*
 * 																*
 * Create a UDP server using standard libc calls and Linux APIs.*
 * The server application must take as only parameter the port 	*
 * number it will listen to. The server will accumulate the 	*
 * information sent by the client. If the server detects a <CR> *
 * caracter (Decimal 13, Hex 0x0D), the server will send to 	*
 * client the Base 64 representation of the data received so 	*
 * far. The server forgets all data before <CR> and starts over.*
 ***************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sockets.h"
#include "encode64.h"
#include "misc.h"
#include "constants.h"


int main(int argc, char *argv[])
{	
  	/************** Validate Arguments **************/
	if((argc != 2) || (!port_number_valid(argv[1])))
	{
		errout("Invalid argument(s)");
	}

	/***************** Socket Setup *****************/
	initialize_socket(atoi(argv[1]));

	/***************** Server Loop ******************/
	// Input and Output buffers start off the same size 	
  	size_t outbuff_index = 0;
  	size_t output_buffer_size = INPUT_BUFFER_SIZE;
  	char input_buffer[INPUT_BUFFER_SIZE];
  	unsigned char *output_buffer = (unsigned char *)malloc(output_buffer_size * sizeof(unsigned char));

  	if(output_buffer == NULL)
  	{
  		errout("Error allocating output buffer!");
  	}

 	while(1) 
 	{
 		size_t chars_read = receive_data(input_buffer, sizeof(input_buffer));

 		// Flag to force a data send even if a <CR> hasn't been received yet
    	char force_send = 0;

    	for(size_t ib_index=0; ib_index < chars_read; ib_index++) 
    	{
    		/***************** Auto Buffer Resize ******************/
    		if(outbuff_index == (output_buffer_size-1))
    		{
    			if(output_buffer_size == MAX_BUFFER_SIZE)
    			{
    				printf("Output buffer full, sending data now...\n");
    				force_send = 1;
    			} else {
    				output_buffer_size += BUFFER_SIZE_INC;
    				output_buffer = realloc(output_buffer, output_buffer_size * sizeof(unsigned char));
    				
    				if(output_buffer == NULL)
  					{
  						errout("Error allocating output buffer!");
  					}
    			}
    		}
    		/*************** End Auto Buffer Resize ****************/

    		if(input_buffer[ib_index] == TERMINATOR_CHAR || force_send)
    		{
    			size_t encoded_length = 0;
    			char *encoded_data = base64_encode(output_buffer, outbuff_index, &encoded_length);

    			if(encoded_data == NULL)
    			{
    				errout("Error allocating encoded data buffer!");	
    			}

    			send_data(encoded_data, encoded_length);
				free(encoded_data);

				// Shrink the output buffer back to its original size
				output_buffer_size = INPUT_BUFFER_SIZE;
				output_buffer = realloc(output_buffer, output_buffer_size * sizeof(unsigned char));
				
				if(output_buffer == NULL)
  				{
  					errout("Error allocating output buffer!");
  				}

				outbuff_index = 0;
    			break;
    		}
    		output_buffer[outbuff_index] = (unsigned char)input_buffer[ib_index];
    		outbuff_index++;
    	}
 	}
 	return 0;
}
