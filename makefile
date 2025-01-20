# Folders
SRC_DIR            		:= ./src
HASH_DIR           		:= $(SRC_DIR)/hash
HASHMAP_DIR            	:= $(SRC_DIR)/hashmap
OBJ_DIR            		:= ./.obj

# Directories
SRC_DIRS := \
    $(SRC_DIR) \
    $(HASH_DIR) \
    $(HASHMAP_DIR) \

# Source files
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
HDRS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))

# Compiler, Flags, Libraries
CC := gcc
CFLAGS := -Wall -std=c99

# Output executable
EXECUTABLE := hash

# Valgrind options
VALGRIND_OPTIONS := --leak-check=full --show-leak-kinds=all

all: clean format build link run

clean:
	@echo "Cleaning"
	@rm -rf $(OBJ_DIR)
	@rm -f $(EXECUTABLE)

format:
	@echo "Formatting"
	@for file in $(SRCS) $(HDRS); do \
		clang-format --style=file -i $$file; \
		if [ $$? -ne 0 ]; then \
			echo "ERROR formatting $$file"; \
			exit 1; \
		fi; \
	done; \

build:
	@echo "Building"
	@mkdir -p $(OBJ_DIR)
	@for file in $(SRCS); do \
		filename=$$(basename $$file .c); \
		$(CC) $(CFLAGS) -c $$file -o $(OBJ_DIR)/$$filename.o; \
		if [ $$? -ne 0 ]; then \
			echo "ERROR building $$file"; \
			exit 1; \
		fi; \
	done

link: 
	@echo "Linking"
	@$(CXX) $(CXXFLAGS) $(wildcard $(OBJ_DIR)/*.o) -o $(EXECUTABLE);
	@if [ $$? -ne 0 ]; then \
		echo "ERROR during linking"; \
		exit 1; \
	fi;
	
run:
	@echo "Running"
	./$(EXECUTABLE)

valgrind:
	@echo "Running Valgrind..."
	@valgrind $(VALGRIND_OPTIONS) ./$(EXECUTABLE)