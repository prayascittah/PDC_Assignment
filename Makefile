# Makefile for Connected Components Assignment
# CSS311 - Parallel and Distributed Computing

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3
OMPFLAGS = -fopenmp

# Targets
SERIAL = serial_cc
PARALLEL = parallel_cc

# Source files
SERIAL_SRC = serial_connected_components.cpp
PARALLEL_SRC = parallel_connected_components.cpp

# Default target
all: $(SERIAL) $(PARALLEL)

# Serial version
$(SERIAL): $(SERIAL_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERIAL) $(SERIAL_SRC)
	@echo "Serial version compiled successfully!"

# Parallel version (with OpenMP)
$(PARALLEL): $(PARALLEL_SRC)
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -o $(PARALLEL) $(PARALLEL_SRC)
	@echo "Parallel version compiled successfully!"

# Run serial version
run-serial: $(SERIAL)
	@echo "Running serial version..."
	./$(SERIAL)

# Run parallel version
run-parallel: $(PARALLEL)
	@echo "Running parallel version..."
	./$(PARALLEL)

# Run both versions
run-all: run-serial run-parallel

# Clean compiled files
clean:
	rm -f $(SERIAL) $(PARALLEL)
	@echo "Cleaned all compiled files"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Compile both serial and parallel versions (default)"
	@echo "  serial_cc    - Compile serial version only"
	@echo "  parallel_cc  - Compile parallel version only"
	@echo "  run-serial   - Compile and run serial version"
	@echo "  run-parallel - Compile and run parallel version"
	@echo "  run-all      - Compile and run both versions"
	@echo "  clean        - Remove compiled files"
	@echo "  help         - Show this help message"

.PHONY: all clean run-serial run-parallel run-all help

