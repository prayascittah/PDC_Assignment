# Makefile for Connected Components Algorithms
# Name: Jayanth P, Roll Number: 2023BCS0040

CC = g++
MPICC = mpic++
CFLAGS = -fopenmp -O2 -std=c++17
MPIFLAGS = -O2 -std=c++17

# Targets
all: serial parallel mpi

serial: serial_connected_components.cpp
	$(CC) $(CFLAGS) -o serial_connected_components serial_connected_components.cpp

parallel: parallel_connected_components.cpp
	$(CC) $(CFLAGS) -o parallel_connected_components parallel_connected_components.cpp

mpi: mpi_connected_components.cpp
	$(MPICC) $(MPIFLAGS) -o mpi_connected_components mpi_connected_components.cpp

# Run targets
run-serial: serial
	./serial_connected_components

run-parallel: parallel
	./parallel_connected_components

run-mpi: mpi
	mpirun -np 4 ./mpi_connected_components

# Clean
clean:
	rm -f serial_connected_components parallel_connected_components mpi_connected_components

.PHONY: all clean run-serial run-parallel run-mpi
