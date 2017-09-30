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
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(SRC_FOLDER)lexical.c  $(SRC_FOLDER)main.c -o $(PROJECT)
string_storage_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)string_storage.c $(TEST_FOLDER)string_storage_test.c -o string_storage_test
	valgrind --leak-check=full ./string_storage_test
	rm string_storage_test
token_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)string_storage.c $(TEST_FOLDER)token_test.c -o token_test
	./token_test
	rm token_test
