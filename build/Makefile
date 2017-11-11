PROJECT = ifj17
PROJECT_TEST = $(PROJECT)-test
BIN_FOLDER = bin/
SRC_FOLDER = ../src/
TEST_FOLDER = ../tests/

CC = gcc
RM = rm -rf

STD_C99 = -std=c99
CFLAGS = -Wall -Wextra -pedantic

all:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c  $(SRC_FOLDER)abstract_tree.c $(SRC_FOLDER)main.c -o $(PROJECT)
string_storage_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)string_storage.c $(TEST_FOLDER)string_storage_test.c -o string_storage_test
	valgrind --leak-check=full ./string_storage_test
	rm string_storage_test
token_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(TEST_FOLDER)token_test.c -o token_test
	./token_test
	rm token_test

stack_test: 
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)stack.c $(TEST_FOLDER)stack_test.c -o stack_test
	valgrind ./stack_test
	rm stack_test

lexan_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c $(TEST_FOLDER)lexical_test.c -o lexan_test
	./lexan_test $(TEST_FOLDER)test_file1.ifj
	rm lexan_test

sym_table_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)symtable.c -o sym_table_test
	./sym_table_test
	rm sym_table_test

