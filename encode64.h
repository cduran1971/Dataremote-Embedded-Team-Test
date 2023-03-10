/****************************************************************
 * Developer:	Celso Duran										*
 * Client:		Dataremote Inc.									*
 * Date:		02/24/2023										*
 * Project: Embedded Team Test - Base64 UDP Server				*
 ***************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);