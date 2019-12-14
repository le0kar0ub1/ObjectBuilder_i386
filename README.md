# ObjectBuilder_i386 is an assembler

**The purpose of the project is to create an assembler for i386 target**

## DEPENDENCY:

	- NASM
	- GCC for x86_64 target
	- ld (if you want finally link the created objects)

## BUILD:

	- make

## RUN:
	- ./run.sh $SourceFile

## THINGS:

	- The only entry symbol allowed is the classical "_start" and it must be declared as global.
	- A list of available instruction will be provided, you can catch all instructions available with a "cat src/parser_code.c".
	- For the moment, you can't define data in `.text` section or put executable instructions in `.data`.
	- You can only declared string and define uninitialize data in section `.data` and `.rodata`.
	- You can't add/sub/mul/div without using instruction ("mov eax, 1 + 1" will not work).
	- You can use symbols declared in `.data` section only in `.text` section.
	- No far jmp allowed, and only 128b jmp allowed... sorry for that, this is a an error.
	- You can Communicate with other file only with the instruction "call".

## EPILOGUE:

	Be nice with the program, this is a little personnal built-in. Clearly this will not work every time.
