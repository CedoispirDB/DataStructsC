# Makefile for DataStructC project

# Set shell to cmd.exe
ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
else
    SHELL := /bin/sh
endif

# Compiler
CC := gcc
# Compiler flags
CFLAGS := -Wall -Wextra -std=c99 -g

# Directories
SRC_DIR := DataStructs
BUILD_DIR := builds
OBJ_DIR := $(BUILD_DIR)/obj
EXE_DIR := $(BUILD_DIR)/bin

# Source files
# SRCS := $(filter-out $(IGNORED_FILES), $(wildcard $(SRC_DIR)/*.c))
SRCS := $(SRC_DIR)/linkedList.c $(SRC_DIR)/hashTable.c
# SRCS := $(SRC_DIR)/xList.c
# Object files
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# OBJS := $(strip $(OBJS))
# Executable name
EXEC := $(EXE_DIR)/main

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
ifeq ($(SHELL), cmd.exe)
	del /Q $(BUILD_DIR)\obj\*.o
	del /Q $(BUILD_DIR)\bin\main.exe
else
	rm -f $(BUILD_DIR)/obj/*
	rm -f $(BUILD_DIR)/bin/*
endif


# ----------------------------------- #
#           Test Commands             #
# ----------------------------------- #

# Name of test file
TEST_SRC := $(SOURCE)
TEST_FILE := $(TEST_SRC)Test

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
ifeq ($(SHELL), cmd.exe)
	del /Q .\Tests\bin\*.exe
	del /Q .\Tests\obj\*.o
else
	rm -f .\Tests\bin\*
	rm -f .\Tests\obj\*
endif


# Clean all 
cleanAll: clean cleanTest

.PHONY: all clean test
