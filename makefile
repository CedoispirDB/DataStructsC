# Makefile for DataStructC project

DATA_NAME := linkedList

# Set shell to cmd.exe
SHELL := cmd.exe

# Compiler
CC := gcc
# Compiler flags
CFLAGS := -Wall -Wextra -std=c99 -g

# Directories
SRC_DIR := DataStructs
OBJ_DIR := Executables

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
# Object files
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# OBJS := $(strip $(OBJS))
# Executable name
EXEC := $(OBJ_DIR)/$(DATA_NAME)

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del .\$(OBJ_DIR)\$(DATA_NAME).o
	del .\$(OBJ_DIR)\$(DATA_NAME).exe

.PHONY: all clean
