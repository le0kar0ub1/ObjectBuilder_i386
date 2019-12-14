##
## EPITECH PROJECT, 2018
## Makefile
## File description:
## project build
##

CC	:=	gcc -g3

NASM	:=	nasm

BIN	:=	ObjectBuilder

BUILDIR	:=	build

LDFLAGS	:=	-L./$(BUILDIR)/mylib -lmy

CFLAGS	:=	-Iinc/BasicHeaders/ -Iinc/x86_opcode/ -Iinc/x86CodeWR/ -Iinc/x86CodeRX/ -Iinc/ElfHeader/ -Iinc/ -Wall -Wextra -g3 -lm

RM	:=	rm -rf

SRC_C   :=	$(wildcard src/*.c src/**/*.c)

OBJ 	:= 	$(patsubst src/%.c, $(BUILDIR)/%.o, $(SRC_C))

.PHONY: all clean fclean run

all:	mylib	$(BIN)

re:	fclean all

mylib:
	@echo -e "BUILDING LIB"
	@make -C lib/ --no-print-directory
	@echo -e "\nBUILDING PROJECT"

$(BIN):	$(OBJ)
	@$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)
	@-echo -e " LINKED      $@"

fclean:
	@$(RM) $(BUILDIR) $(BIN) vgcore.*

$(BUILDIR)/%.o: src/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@-echo -e "     CC      $@"

run:
	@./$(BIN)
