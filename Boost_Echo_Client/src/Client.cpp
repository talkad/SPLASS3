#include <connectionHandler.h>
#include <UserData.h>
#include <boost/asio.hpp>
#include <thread>

using std::string;

void writeTask(ConnectionHandler* connectionHandler){
    while (true) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        string line(buf);
        string frameOut=connectionHandler->toStompFrame(line);
        if (!connectionHandler->sendFrame(frameOut)) {
            break;
        }
    }
}

void readTask(ConnectionHandler* connectionHandler){
    while (true) {
        if(connectionHandler->connected()) {
            string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
            if (!connectionHandler->getFrame(answer)) {
                break;
            }

            string sendMsg = connectionHandler->process(answer);
            if (sendMsg.length() > 0) { //there is a response to the server
                if (!connectionHandler->sendFrame(sendMsg)) {
                    break;
                }
            }
        }
    }
}



using namespace boost;

int main (int argc, char *argv[]) {
    
    ConnectionHandler* connection = new ConnectionHandler();

    std::thread thread_1 = std::thread(writeTask, connection);
    //std::thread thread_2 = std::thread(readTask, connection);

    //thread_2.join();
    thread_1.join();

    //TODO: delete this pointer at the end of the connection- delete UserData::getInstance()
    delete connection;

    return 0;
}

