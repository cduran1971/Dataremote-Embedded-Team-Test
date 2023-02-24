/****************************************************************
 * Developer:	Celso Duran										*
 * Client:		Dataremote Inc.									*
 * Date:		02/24/2023										*
 * Project: Embedded Team Test - Base64 UDP Server				*
 ***************************************************************/

#define TERMINATOR_CHAR 13		// 13 = <CR>

#define MAX_PORT_NUMBER 65535
#define MAX_BIND_RETRIES 2
#define INPUT_BUFFER_SIZE 512
#define MAX_BUFFER_SIZE 2048	// The maximum size in bytes the output buffer will be allowed to grow
#define BUFFER_SIZE_INC 512		// The step size of the output buffer increments