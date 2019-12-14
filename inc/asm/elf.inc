%ifndef _ELF_INC_
%define _ELF_INC_

/* https://en.wikipedia.org/wiki/Executable_and_Linkable_Format */

/* /!\/!\/!\ ELF HEADER /!\/!\/!\ */
        ; MAGIC ELF NBR
        %define EI_MAG0 0x7F
        %define EI_MAG1 0x45
        %define EI_MAG2 0x4C
        %define EI_MAG3 0x46

        ; SOME VALUE DESCRIBING FILE
        ; 1 == 32b / 2 == 64b
        %define EI_CLASS      0x4
        ; 1 == Little Endian / 2 == Big Endian
        %define EI_DATA       0x5
        ; original and current version elf == 1
        %define EI_VERSION    0x6
        ; target ABI
        %define EI_OSABI      0x7
        ; depend target ABI
        %define EI_ABIVERSION 0x8
        ; padding 7 byte
        %define EI_PAD        0x9
        ; object file type
        %define E_TYPE        0x10
        ; target instr arch
        %define E_MACHINE     0x12
        ; original elf version
        %define E_VERSION     0x14

        /* Header entry sections points */

%endif
