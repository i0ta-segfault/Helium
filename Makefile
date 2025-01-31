CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

SRC_DIR = src
SRC_FILES = $(SRC_DIR)/main.c \
            $(SRC_DIR)/lexer/lexer.c \
            $(SRC_DIR)/parser/parser.c \
            $(SRC_DIR)/token/token.c \
            $(SRC_DIR)/ast/ast.c \
            $(SRC_DIR)/evaluator/evaluator.c \
            $(SRC_DIR)/utils/hashmap.c
TARGET = bin/main
SOURCE_CODE = main.he

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	gcc -I./include -g -o $(TARGET) $(SRC_FILES) -lm

run: $(TARGET)
	cd bin && ./main $(SOURCE_CODE)

clean:
	rm -f $(TARGET)
