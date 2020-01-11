#include <stdlib.h>
#include <connectionHandler.h>
#include <UserData.h>
#include <Frame.h>
#include <boost/thread.hpp>

/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
using std::string;

void writeTask(ConnectionHandler connectionHandler){
    while (true) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        string line(buf);
        string frameOut=connectionHandler.toStompFrame(line);
        if (!connectionHandler.sendFrame(frameOut)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
    }
}

void readTask(ConnectionHandler connectionHandler){
    while (true) {
        string answer;
        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
        if (!connectionHandler.getFrame(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }

        Frame frameIN(answer);
        string sendMsg=connectionHandler.process(frameIN);
        if(sendMsg.length()>0){ //there is a response to the server
            if (!connectionHandler.sendFrame(sendMsg)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
    }
}




int main (int argc, char *argv[]) {
    if (argc < 2) {
        return -1;
    }
    string host = argv[0];
    short port = atoi(argv[1]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        printf("Could not connect to server");
        return 1;
    }

    using namespace boost;
    thread thread_1 = thread(writeTask,connectionHandler);
    thread thread_2 = thread(readTask,connectionHandler);

    thread_2.join();
    thread_1.join();


//    while (1) {
//        const short bufsize = 1024;
//        char buf[bufsize];
//        std::cin.getline(buf, bufsize);
//		string line(buf);
//		int len=line.length();
//		string frameOUT=Frame(line).toString();
//        if (!connectionHandler.sendFrame(frameOUT)) {
//            std::cout << "Disconnected. Exiting...\n" << std::endl;
//            break;
//        }
//		// connectionHandler.sendLine(line) appends '\n' to the message. Therefor we send len+1 bytes.
//        std::cout << "Sent " << len+1 << " bytes to server" << std::endl;
//
//
//        // We can use one of three options to read data from the server:
//        // 1. Read a fixed number of characters
//        // 2. Read a line (up to the newline character using the getline() buffered reader
//        // 3. Read up to the null character
//        string answer;
//        // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
//        // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
//        if (!connectionHandler.getFrame(answer)) {
//            std::cout << "Disconnected. Exiting...\n" << std::endl;
//            break;
//        }
//
//        Frame frameIN(answer);
//
//
//
//		len=answer.length();
//		// A C string must end with a 0 char delimiter.  When we filled the answer buffer from the socket
//		// we filled up to the \n char - we must make sure now that a 0 char is also present. So we truncate last character.
//        answer.resize(len-1);
//        std::cout << "Reply: " << answer << " " << len << " bytes " << std::endl << std::endl;
//        if (answer == "bye") {
//            std::cout << "Exiting...\n" << std::endl;
//            break;
//        }
//    }
    return 0;
}

