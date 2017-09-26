# <<<<<<< HEAD
PROJECT = ifj17
PROJECT_TEST = $(PROJECT)-test
BIN_FOLDER = bin/
SRC_FOLDER = ../src/
TESTS_FOLDER = ../tests/

CC = gcc
RM = rm -rf

STD_C99 = -std=c99
CFLAGS = -Wall -Wextra -pedantic -g

all:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(SRC_FOLDER)lexical2.c  $(SRC_FOLDER)main.c -o $(PROJECT)

token_test:
	$(CC) $(STD_C99) $(CFLAGS) $(SRC_FOLDER)token.c $(TEST_FOLDER)token_test.c -o token_test
	valgrind --leak-check=full ./token_test
	rm token_test	
