/****************************************************************
 * Developer:	Celso Duran										*
 * Client:		Dataremote Inc.									*
 * Date:		02/24/2023										*
 * Project: Embedded Team Test - Base64 UDP Server				*
 ***************************************************************/

#include "misc.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errout(char *err_string) 
{
	printf("%s\n", err_string);
	exit(EXIT_FAILURE);
}

int port_number_valid(char *a_string)
{
	int length = strlen(a_string);

	// Port numbers have at most 5 digits
	if(length > 6)
	{
		return 0;
	}

	for(int i=0; i < length; ++i)
	{
		if(a_string[i] < '0' || a_string[i] > '9' )
			return 0;
	}

	if(atoi(a_string) > MAX_PORT_NUMBER)
	{
		return 0;
	}

	return 1;
}