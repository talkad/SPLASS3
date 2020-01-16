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
        if (connectionHandler->isLoggedIn()) {
            connectionHandler->sendFrame(frameOut);
            if(line=="logout") {
                connectionHandler->setLogin(false);
                connectionHandler->close();
            }
        }
    }
}
//132.72.234.65:7777
void readTask(ConnectionHandler* connectionHandler){
    while (connectionHandler->isRunning()) {
        if(connectionHandler->isLoggedIn()) {
            string answer;
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
    std::thread thread_2 = std::thread(readTask, connection);

    thread_2.join();
    thread_1.join();

    delete connection;

    return 0;
}

