# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11 -pthread
LFLAGS  = -L/usr/libz

# All Targets
all: client

# Tool invocations
# Executable "client" depends on
client: bin/Client.o bin/connetionHandler.o bin/StompEncoderDecoder.o bin/StompMessagingProtocol.o bin/UserData.o bin/thread.o
	@echo 'Building target: client'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/client bin/Client.o bin/connetionHandler.o bin/StompEncoderDecoder.o bin/StompMessagingProtocol.o bin/UserData.o bin/thread.o
	@echo 'Finished building target: client'
	@echo ' '

#target objects files
bin/Client.o: src/Client.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Client.o src/Client.cpp

bin/connetionHandler.o: src/connetionHandler.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/connetionHandler.o src/connetionHandler.cpp

bin/StompEncoderDecoder.o: src/StompEncoderDecoder.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/StompEncoderDecoder.o src/StompEncoderDecoder.cpp

bin/StompMessagingProtocol.o: src/StompMessagingProtocol.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/StompMessagingProtocol.o src/StompMessagingProtocol.cpp

bin/UserData.o: src/UserData.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/UserData.o src/UserData.cpp

bin/thread.o: boost/thread.hpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/thread.o -x c++ boost/thread.hpp

#Clean the build directory
clean:
	rm -f bin/*


