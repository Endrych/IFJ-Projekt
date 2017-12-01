PROJECT = ifj17

CC = gcc
RM = rm -rf
FOLDER = 
SHELL := /bin/bash
STD_C99 = -std=c99
CFLAGS = -Wall -Wextra -pedantic -g

all:
	$(CC) $(STD_C99) $(CFLAGS) $(FOLDER)built_in.c $(FOLDER)abstract_tree.c $(FOLDER)gen_stacks.c $(FOLDER)stack.c $(FOLDER)string_storage.c $(FOLDER)set_values.c $(FOLDER)at_que.c $(FOLDER)generator.c $(FOLDER)destructor.c $(FOLDER)frame.c $(FOLDER)lexical.c $(FOLDER)error.c $(FOLDER)symtable.c $(FOLDER)precedence_sa.c $(FOLDER)token.c $(FOLDER)parser.c  $(FOLDER)main.c -o main
	
test: 
	source testy/tester.sh main