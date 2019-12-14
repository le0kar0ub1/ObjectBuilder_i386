#!/bin/bash

if [ $# -ne 1 ]; then
    echo "./run.sh SourceFile"
    exit
fi

bin=$1
out=$(echo $bin | cut -d. -f1)
obj="$out.o"

make

if [ $? -ne 0 ]; then
    exit
fi

./ObjectBuilder $bin

if [ $? -ne 0 ]; then
    exit
fi

ld -m elf_i386 $obj -o $out

if [ $? -ne 0 ]; then
    exit
fi

./$out

#echo $?
