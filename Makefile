# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I src/headers -g

# Libraries
LDFLAGS = -ljson-c

# Directories
SRC_DIR = src
OBJ_DIR = build/objects
BUILD_DIR = build
RESOURCES_DIR = resources
INSTALL_DIR = /usr/local/bin

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Output binary
TARGET = $(BUILD_DIR)/ftrackr

# Default target
all: setup $(TARGET)

# Create directories
setup:
	mkdir -p $(OBJ_DIR) $(BUILD_DIR) $(RESOURCES_DIR)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files to objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the build directory
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

# Clean everything including resources
dist-clean: clean
	rm -f $(RESOURCES_DIR)/*

# Install the application
install: $(TARGET)
	install -m 755 $(TARGET) $(DESTDIR)$(INSTALL_DIR)/ftrackr
	@echo "Installed to $(DESTDIR)$(INSTALL_DIR)/ftrackr"

# Uninstall the application
uninstall:
	rm -f $(DESTDIR)$(INSTALL_DIR)/ftrackr
	@echo "Uninstalled from $(DESTDIR)$(INSTALL_DIR)/ftrackr"

# PHONY targets
.PHONY: all clean dist-clean setup #install uninstall
