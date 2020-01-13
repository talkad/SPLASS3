# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/libz -lpthread -lboost

# All Targets
all: client

# Tool invocations
# Executable "client" depends on
client: bin/Client.o bin/connectionHandler.o bin/StompEncoderDecoder.o bin/StompMessagingProtocol.o bin/UserData.o
	@echo 'Building target: client'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/client bin/Client.o bin/connectionHandler.o bin/StompEncoderDecoder.o bin/StompMessagingProtocol.o bin/UserData.o
	@echo 'Finished building target: client'
	@echo ' '

#target objects files
bin/Client.o: src/Client.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Client.o src/Client.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

bin/StompEncoderDecoder.o: src/StompEncoderDecoder.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/StompEncoderDecoder.o src/StompEncoderDecoder.cpp

bin/StompMessagingProtocol.o: src/StompMessagingProtocol.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/StompMessagingProtocol.o src/StompMessagingProtocol.cpp

bin/UserData.o: src/UserData.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/UserData.o src/UserData.cpp

#Clean the build directory
clean:
	rm -f bin/*


