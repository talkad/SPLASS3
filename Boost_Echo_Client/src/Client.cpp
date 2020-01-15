#include <connectionHandler.h>
#include <UserData.h>
#include <thread>

//#include <boost/asio.hpp>
using std::string;

void writeTask(ConnectionHandler* connectionHandler){
    while (connectionHandler->isRunning()) {
        const short bufsize = 1024;
        char buf[bufsize];
        std::cin.getline(buf, bufsize);
        string line(buf);
        if(line=="bye")
            connectionHandler->terminate();

        string frameOut=connectionHandler->toStompFrame(line);

        if (connectionHandler->connected()) {
            connectionHandler->sendFrame(frameOut);
            if(line=="logout") {
                connectionHandler->setConnected(false);
            }
        }
    }
}
//192.168.43.45:7777
void readTask(ConnectionHandler* connectionHandler){
    while (connectionHandler->isRunning()) {
        if(connectionHandler->connected()) {
            string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            if (!connectionHandler->getFrame(answer)) {
                break;
            }

            //printf("xxxxx \n %s \n xxxxxxxx \n",answer.c_str());

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
    std::thread thread_2 = std::thread(readTask, connection);

    thread_2.join();
    thread_1.join();

    //TODO: delete this pointer at the end of the connection- delete UserData::getInstance()
    delete connection;

    return 0;
}

