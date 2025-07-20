# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -Iinclude -Isrc

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c) \
          $(wildcard $(SRCDIR)/algorithms/*.c) \
          $(wildcard $(SRCDIR)/simulator/*.c) \
          $(wildcard $(SRCDIR)/utils/*.c)

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Main executable
TARGET = $(BINDIR)/cpu_scheduler

# Test files
TEST_SOURCES = $(wildcard $(TESTDIR)/*.c)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(OBJDIR)/tests/%.o)
TEST_TARGETS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(BINDIR)/%)

# Examples
EXAMPLE_SOURCES = $(wildcard examples/*.c)
EXAMPLE_TARGETS = $(EXAMPLE_SOURCES:examples/%.c=$(BINDIR)/%)

.PHONY: all clean test examples help

# Default target
all: $(TARGET)

# Create directories
$(OBJDIR):
	mkdir -p $(OBJDIR)/algorithms $(OBJDIR)/simulator $(OBJDIR)/utils $(OBJDIR)/tests

$(BINDIR):
	mkdir -p $(BINDIR)

# Build main executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build tests
test: $(TEST_TARGETS)

$(BINDIR)/test_%: $(OBJDIR)/tests/test_%.o $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) | $(BINDIR)
	$(CC) $^ -o $@

$(OBJDIR)/tests/%.o: $(TESTDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build examples
examples: $(EXAMPLE_TARGETS)

$(BINDIR)/%: examples/%.c $(filter-out $(OBJDIR)/main.o, $(OBJECTS)) | $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# Run all tests
run-tests: test
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		$$test; \
	done

# Clean build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build main executable"
	@echo "  test      - Build test executables"
	@echo "  examples  - Build example programs"
	@echo "  run-tests - Build and run all tests"
	@echo "  clean     - Remove build files"
	@echo "  help      - Show this help message"