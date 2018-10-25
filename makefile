##
# PROGRAM:    Assignment 5
# PROGRAMMER: Tyler Saballus
# LOGON ID:   z1836409
# DATE DUE:   10-26-2018
#

# Compiler variables
CC = g++
CCFLAGS = -Wall -pthread

# Rule to link object code files to create executable file
Assign5: hw5.o
	$(CC) $(CCFLAGS) -o Assign5 hw5.o

# Rules to compile source code file to object code
hw5.o: hw5.cc
	$(CC) $(CCFLAGS) -c hw5.cc

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign5
