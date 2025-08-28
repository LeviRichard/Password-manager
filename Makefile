# PasswordVault Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUG_FLAGS = -g -DDEBUG -O0
LIBS = -ljson-c -lssl -lcrypto

# Target executable
TARGET = PasswordVault

# Source and object files
SRCDIR = .
SOURCES = main.c hash.c database_operations.c encrypt_decrypt.c password.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = database_operations.h encrypt_decrypt.h hash.h password.h

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@chmod +x $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "Debug build complete"

# Release build with optimizations
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)
	@echo "Release build complete"

# Install target (optional)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin/"
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Uninstall target (optional)
uninstall:
	@echo "Removing $(TARGET) from /usr/local/bin/"
	@sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Clean everything including backup files
distclean: clean
	@echo "Deep cleaning..."
	@rm -f *~ *.bak *.tmp core

# Check for required libraries
check-deps:
	@echo "Checking dependencies..."
	@pkg-config --exists json-c || (echo "Error: json-c library not found" && exit 1)
	@pkg-config --exists openssl || (echo "Error: OpenSSL library not found" && exit 1)
	@echo "All dependencies found"

# Run static analysis (requires cppcheck)
analyze:
	@echo "Running static analysis..."
	@which cppcheck > /dev/null || (echo "cppcheck not installed" && exit 1)
	@cppcheck --enable=all --std=c99 $(SOURCES)

# Format code (requires clang-format)
format:
	@echo "Formatting source code..."
	@which clang-format > /dev/null || (echo "clang-format not installed" && exit 1)
	@clang-format -i $(SOURCES) $(HEADERS)
	@echo "Code formatting complete"

# Show help
help:
	@echo "PasswordVault Makefile"
	@echo "======================"
	@echo "Available targets:"
	@echo "  all       - Build the application (default)"
	@echo "  debug     - Build with debug symbols and no optimization"
	@echo "  release   - Build optimized release version"
	@echo "  install   - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall - Remove from /usr/local/bin (requires sudo)"
	@echo "  clean     - Remove build artifacts"
	@echo "  distclean - Remove all generated files"
	@echo "  check-deps- Check if required libraries are installed"
	@echo "  analyze   - Run static code analysis (requires cppcheck)"
	@echo "  format    - Format source code (requires clang-format)"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build normally"
	@echo "  make debug        # Build with debugging"
	@echo "  make release      # Build optimized version"
	@echo "  make clean all    # Clean and rebuild"

# Declare phony targets
.PHONY: all debug release install uninstall clean distclean check-deps analyze format help

# Show variables (for debugging the Makefile)
show-vars:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LIBS: $(LIBS)"
	@echo "TARGET: $(TARGET)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "HEADERS: $(HEADERS)"