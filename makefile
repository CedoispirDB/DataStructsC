# Makefile for DataStructC project

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
EXEC := $(OBJ_DIR)/main

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del .\$(OBJ_DIR)\*.o
	del .\$(OBJ_DIR)\main.exe


# ----------------------------------- #
#           Test Commands             #
# ----------------------------------- #

# Name of test file
TEST_FILE := linkedListTest
TEST_SRC := linkedList

# Object files directory
OBJ_DIR = Tests/obj

# Executables directory
BIN_DIR = Tests/bin

# Source files
TEST_SRCS = Tests/$(TEST_FILE).c
LIST_SRCS = DataStructs/$(TEST_SRC).c

# Object files
TEST_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(TEST_SRCS)))
LIST_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(LIST_SRCS)))

# Executables
TEST_EXEC = $(BIN_DIR)/$(TEST_FILE)

# Default target
test: $(TEST_EXEC)

# Compile test file
$(OBJ_DIR)/$(TEST_FILE).o: Tests/$(TEST_FILE).c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile src file
$(OBJ_DIR)/$(TEST_SRC).o: DataStructs/$(TEST_SRC).c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files and generate the executable
$(TEST_EXEC): $(TEST_OBJS) $(LIST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

cleanTest:
	del .\Tests\bin\*.exe
	del .\Tests\obj\*.o

.PHONY: all clean
