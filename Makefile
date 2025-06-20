# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUGFLAGS = -g -DDEBUG

# Directories
SRCDIR = src
BUILDDIR = build
BINDIR = $(BUILDDIR)/bin

# Files
SOURCES = $(SRCDIR)/main.c
OBJECTS = $(BUILDDIR)/main.o
TARGET = $(BINDIR)/dodger
TARGET_DEBUG = $(BINDIR)/dodger_debug

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Compile object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link the final executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Debug build
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET_DEBUG)

$(TARGET_DEBUG): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(OBJECTS) -o $@

# Clean build files
clean:
	rm -rf $(BUILDDIR)

# Clean and rebuild
rebuild: clean all

# Run the game
run: $(TARGET)
	./$(TARGET)

# Run debug version
run-debug: debug
	./$(TARGET_DEBUG)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/dodger

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/dodger

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the game (default)"
	@echo "  debug     - Build debug version"
	@echo "  clean     - Remove build files"
	@echo "  rebuild   - Clean and build"
	@echo "  run       - Build and run the game"
	@echo "  run-debug - Build and run debug version"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  help      - Show this help message"

# Phony targets
.PHONY: all debug clean rebuild run run-debug install uninstall help 