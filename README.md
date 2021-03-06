# ObjectBuilder_i386 is an assembler

**The purpose of the project is to create an assembler for i386 target**

## DEPENDENCY:
   * nasm
   * gcc
   * ld (if you want finally link the created objects)

## BUILD:

`make`

## RUN:

`./run.sh $asmfile`

Where `$asmfile` is your assembly source file.

## THINGS:

About the assembler:
  * The only entry symbol allowed is the classical `_start` and it must be declared as global.
  * A list of available instruction will be provided, you can catch all instructions available with a `cat src/parser_code.c`.
  * For the moment, you can't define data in `.text` section or put executable instructions in `.data`.
  * You can only declare string and define uninitialized data in section `.data` and `.rodata`.
  * You can't add/sub/mul/div without using instruction (`mov eax, 1 + 1` will not work).
  * You can use symbols declared in `.data` section only in `.text` section.
  * No far jump allowed, and 128B maximal jump size... come from error.
  * You can Communicate with other file only with the instruction `call`.

## EPILOGUE:

A little project without ambition for the fun. Clearly this will not work every time.
