##
## EPITECH PROJECT, 2018
## Makefile
## File description:
## library build
##

CC	:=	gcc -g3 #-coverage

ARRC	:=	ar rc

LIB	:=	libmy.a

LDIR	:=	../build/mylib

CFLAGS	:=	-I../inc/BasicHeaders/ -g -Wall -Wextra

RM	:=	rm -rf

SRC :=	$(wildcard src/*.c)

OBJ := 	$(patsubst src/%.c, $(LDIR)/%.o, $(SRC))

.PHONY: all re fclean

all:	$(LIB)

fclean:
	$(RM) $(LDIR)

re:	fclean all

$(LIB):	$(OBJ)
	@$(ARRC) $(LIB) $(OBJ)
	@echo -e " LINKED      $@"
	@mv $(LIB) $(LDIR)

$(LDIR)/%.o: src/%.c
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@-echo -e "     CC      $@"
