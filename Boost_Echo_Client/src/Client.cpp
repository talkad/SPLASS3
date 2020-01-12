#include <cstdlib>
#include <connectionHandler.h>
#include <UserData.h>
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

        string sendMsg=connectionHandler.process(answer);
        if(sendMsg.length()>0){ //there is a response to the server
            if (!connectionHandler.sendFrame(sendMsg)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
        }
    }
}




int main (int argc, char *argv[]) {
    
    ConnectionHandler connectionRead(host, port);
    ConnectionHandler connectionWrite(host,port);
    if (!connectionRead.connect() && !connectionWrite.connect()) {
        printf("Could not connect to server");
        return 1;
    }

    using namespace boost;

//    thread thread_1 = thread(writeTask, &connectionWrite);
//    thread thread_2 = thread(readTask, &connectionRead);
//
//    thread_2.join();
//    thread_1.join();

    return 0;
}

