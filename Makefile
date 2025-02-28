CC = g++
CFLAGS = -Wall -lrt

# Shared Memory Processes (A, B, C) - Located in the main directory
SHM_A = process_A
SHM_B = process_B
SHM_C = process_C

# Fork Implementations (Nested & Separate) - Located in the main directory
FORK_NESTED = forks_nested
FORK_SEPARATE = forks_separate

# Message Queue Client & Server - Located inside respective directories
MQCLIENT = MQclient/client-1
MQSERVER = MQserver/server-1

# Build everything
all: client server shm_processes fork_processes

# Compiling Shared Memory Processes (A, B, C)
shm_processes: $(SHM_A) $(SHM_B) $(SHM_C)

$(SHM_A): process_A.cpp
	$(CC) $(CFLAGS) process_A.cpp -o $(SHM_A)

$(SHM_B): process_B.cpp
	$(CC) $(CFLAGS) process_B.cpp -o $(SHM_B)

$(SHM_C): process_C.cpp
	$(CC) $(CFLAGS) process_C.cpp -o $(SHM_C)

# Compiling Fork Implementations (Nested & Separate)
fork_processes: $(FORK_NESTED) $(FORK_SEPARATE)

$(FORK_NESTED): forks_nested.cpp
	$(CC) $(CFLAGS) forks_nested.cpp -o $(FORK_NESTED)

$(FORK_SEPARATE): forks_separate.cpp
	$(CC) $(CFLAGS) forks_separate.cpp -o $(FORK_SEPARATE)

# Compiling Message Queue Client & Server
client: MQclient/client-1.cpp
	$(CC) $(CFLAGS) MQclient/client-1.cpp -o $(MQCLIENT)

server: MQserver/server-1.cpp
	$(CC) $(CFLAGS) MQserver/server-1.cpp -o $(MQSERVER)

# Clean up all compiled files
clean:
	rm -f $(SHM_A) $(SHM_B) $(SHM_C) $(FORK_NESTED) $(FORK_SEPARATE) $(MQCLIENT) $(MQSERVER)
