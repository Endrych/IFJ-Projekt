PROJECT = ifj17
PROJECT_TEST = $(PROJECT)-test
BIN_FOLDER = bin/
SRC_FOLDER = ../src/
TEST_FOLDER = ../tests/

CC = gcc
RM = rm -rf

STD_C99 = -std=c99
CFLAGS = -Wall -Wextra -pedantic -g

all:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c $(SRC_FOLDER)set_values.c $(SRC_FOLDER)frame.c $(SRC_FOLDER)abstract_tree.c $(SRC_FOLDER)main.c -o $(PROJECT)
string_storage_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)string_storage.c $(TEST_FOLDER)string_storage_test.c -o string_storage_test
	valgrind --leak-check=full ./string_storage_test
	rm string_storage_test
token_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(TEST_FOLDER)token_test.c -o token_test
	valgrind ./token_test
	rm token_test

stack_test: 
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)stack.c $(TEST_FOLDER)stack_test.c -o stack_test
	valgrind --leak-check=full ./stack_test
	rm stack_test

stack_test_coverage:
	$(CC) $(STD_C99) $(CFLAGS) --coverage $(SRC_FOLDER)token.c $(SRC_FOLDER)stack.c $(TEST_FOLDER)stack_test.c -o stack_test
	./stack_test
	lcov --capture --directory . --output-file stack_coverage.info
	genhtml stack_coverage.info --output-directory ./stack/
	rm stack_test *.gcda *gcno *.info

lexan_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c $(TEST_FOLDER)lexical_test.c -o lexan_test
	./lexan_test $(TEST_FOLDER)test_file1.ifj
	rm lexan_test

precedence_test_coverage:
	$(CC) $(STD_C99) $(CFLAGS) --coverage $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c $(SRC_FOLDER)token.c $(SRC_FOLDER)abstract_tree.c $(SRC_FOLDER)stack.c $(SRC_FOLDER)symtable.c $(SRC_FOLDER)precedence_sa.c $(SRC_FOLDER)at_que.c $(TEST_FOLDER)precedence_sa_test.c -o precedence_test
	./precedence_test

sym_table_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)set_values.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)symtable.c -o sym_table_test
	valgrind ./sym_table_test
	rm sym_table_test

abstract_tree_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)abstract_tree.c $(TEST_FOLDER)abstract_tree_test.c -o abstract_tree_test
	valgrind ./abstract_tree_test
	rm abstract_tree_test

frame_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)frame.c -o frame_test
	./frame_test
	rm frame_test

parser: 
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)lexical.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)parser.c $(SRC_FOLDER)set_values.c $(SRC_FOLDER)symtable.c $(SRC_FOLDER)stack.c $(SRC_FOLDER)precedence_sa.c $(SRC_FOLDER)abstract_tree.c  -g -o parser
generator:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c $(SRC_FOLDER)set_values.c $(SRC_FOLDER)frame.c $(SRC_FOLDER)abstract_tree.c $(SRC_FOLDER)stack.c $(SRC_FOLDER)gen_stacks.c $(SRC_FOLDER)symtable.c $(SRC_FOLDER)generator.c $(SRC_FOLDER)name_generator.c $(TEST_FOLDER)generator_test.c -o generator_test
	./generator_test
name_generator:
	$(CC) $(STD_C99) $(CFLAGS) $(TEST_FOLDER)name_generator_test.c $(SRC_FOLDER)name_generator.c -o name_generator
	./name_generator
	rm name_generator

generator_test:
	$(CC) $(STD_C99) $(CFLAGS) --coverage $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c $(SRC_FOLDER)token.c $(SRC_FOLDER)abstract_tree.c $(SRC_FOLDER)stack.c $(SRC_FOLDER)generator.c $(SRC_FOLDER)name_generator.c $(SRC_FOLDER)gen_stacks.c $(SRC_FOLDER)symtable.c $(SRC_FOLDER)at_que.c $(SRC_FOLDER)precedence_sa.c $(TEST_FOLDER)gen_test.c -o generator_test
	./generator_test
	rm generator_test
