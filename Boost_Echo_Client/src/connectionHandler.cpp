#include <connectionHandler.h>
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(): io_service_(), socket_(io_service_), encdec(), protocol(),
        runFlag(true),isConnect(false){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(UserData::getInstance()->getHost()), UserData::getInstance()->getPort()); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
		isConnect=true;
    }
    catch (std::exception& e) {
        std::cout<< "Could not connect to server" << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp) {
            tmp += socket_.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
        }
        if (error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        printf("connection closed! \n");
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error) {
            throw boost::system::system_error(error);
        }
    } catch (std::exception& e) {
        printf("connection closed! \n");
        return false;
    }
    return true;
}

bool ConnectionHandler::getFrame(std::string &frame) {
    return getFrameAscii(frame, '\0');
}

bool ConnectionHandler::sendFrame(std::string &frame) {
    return sendFrameAscii(frame, '\0');
}

bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    try {
	do{
		if(!getBytes(&ch, 1))
		{
			return false;
		}
		if(ch!='\0')
			frame.append(1, ch);
	}while (delimiter != ch);
    } catch (std::exception& e) {
        printf("connection closed ! \n");
        //std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
	    return false;
    }
    return true;
}

bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
	bool result=sendBytes(frame.c_str(),frame.length());
	if(!result) return false;
    return sendBytes(&delimiter,1); //sends the delimiter
}

// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
        isConnect=false;
    } catch (...) {
        printf("closing failed: connection already closed");
    }
}

string ConnectionHandler::process(string& frame) {
    return protocol.process(frame);
}

string ConnectionHandler::toStompFrame(string& msg) {
    string result=encdec.toStompFrame(msg);
    if(!isConnect && UserData::getInstance()!= nullptr && UserData::getInstance()->getHost().length()>0) {//check this line
        connect();
    }
    return result;
}

bool ConnectionHandler::isRunning() {
    return runFlag;
}

void ConnectionHandler::terminate() {
    runFlag=false;
}

bool ConnectionHandler::isConnected() {
    return isConnect;
}
