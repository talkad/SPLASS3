#include "StompEncoderDecoder.h"
#include <unordered_map>
#include <connectionHandler.h>

int StompEncoderDecoder::indexOf(string &text, string &pattern) {
    std::string::size_type loc = text.find(pattern, 0);
    if(loc != std::string::npos)
    {
        return loc;
    }
    else
    {
        return -1;
    }
}

string StompEncoderDecoder::toStompFrame(const string& msg) {
    string frame;
    vector<string> wordsVector;
    splitSentence(msg, wordsVector);
    if(wordsVector.size()>0) {
    string command=wordsVector.at(0);
        if (command == "login")
            frame = loginFrame(wordsVector);
        else if (command == "join")
            frame = joinFrame(wordsVector);
        else if (command == "exit")
            frame = exitFrame(wordsVector);
        else if (command == "add")
            frame = addFrame(wordsVector);
        else if (command == "borrow")//the response is handle in the protocol
            frame = borrowFrame(wordsVector);
        else if (command == "return")
            frame = returnFrame(wordsVector);
        else if (command == "status") //the response is handle in the protocol
            frame = statusFrame(wordsVector);
        else if (command == "logout")
            frame = logoutFrame(wordsVector);
    }
    return frame;
}

string StompEncoderDecoder::loginFrame(vector<string> wordsVector) {
    string frame;
    string colon=":";
    int index=indexOf(wordsVector.at(1),colon);
    string host=wordsVector.at(1).substr(0,index);
    string port=wordsVector.at(1).substr(index+1);
    frame+="CONNECT\n";
    frame+="accept-version: 1.2\n";
    frame+="host:"+wordsVector.at(1)+"\n";
    frame+="login:"+wordsVector.at(2)+"\n";
    UserData::initiate(wordsVector.at(2),host,std::stoi(port));
    frame+="passcode:"+wordsVector.at(3)+"\n";
    frame+="\n";

    return frame;
}

string StompEncoderDecoder::joinFrame(vector<string> wordsVector) {
    string frame;
    frame+="SUBSCRIBE\n";
    frame+="destination:"+wordsVector.at(1)+"\n";
    int id=UserData::getInstance()->generateSubID();
    frame+="id:"+ std::to_string(id)+"\n";
    frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
    frame+="\n";
    UserData::getInstance()->addSubscription(wordsVector.at(1), id);

    return frame;
}

string StompEncoderDecoder::exitFrame(vector<string> wordsVector) {
    string frame;
    frame+="UNSUBSCRIBE\n";
    frame+="destination:"+wordsVector.at(1)+"\n";
    frame+="id:"+ std::to_string(UserData::getInstance()->getSubByGenre(wordsVector.at(1)))+"\n";
    frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
    frame+="\n";

    return frame;
}

string StompEncoderDecoder::addFrame(vector<string> wordsVector) {
    string frame;
    frame+="SEND\n";
    frame+="destination:"+wordsVector.at(1)+"\n";
    frame+="\n";
    string bookName;
    for(size_t i=2;i<wordsVector.size();i++){
        bookName+=wordsVector[i];
        if(i+1<wordsVector.size())
            bookName+=" ";
    }
    frame+= UserData::getInstance()->getName() +" has added the book "+ bookName+"\n";
    UserData::getInstance()->addBook(wordsVector.at(1),bookName);

    return frame;
}

string StompEncoderDecoder::borrowFrame(vector<string> wordsVector) {
    string frame;
    frame+="SEND\n";
    frame+="destination:"+wordsVector.at(1)+"\n";
    frame+="\n";
    string bookName;
    for(size_t i=2;i<wordsVector.size();i++){
        bookName+=wordsVector[i];
        if(i+1<wordsVector.size())
            bookName+=" ";
    }
    frame+= UserData::getInstance()->getName() +" wish to borrow "+bookName;
    frame+="\n";

    return frame;
}

string StompEncoderDecoder::returnFrame(vector<string> wordsVector) {
    string frame;
    frame+="SEND\n";
    frame+="destination:"+wordsVector.at(1)+"\n";
    frame+="\n";
    string bookName;
    for(size_t i=2;i<wordsVector.size();i++){
        bookName+=wordsVector[i];
        if(i+1<wordsVector.size())
            bookName+=" ";
    }
    frame+="Returning "+ bookName + " to " + UserData::getInstance()->getLender(bookName)+"\n";
    UserData::getInstance()->remove(wordsVector.at(1), bookName);
    UserData::getInstance()-> removeBorrow(bookName);

    return frame;
}

string StompEncoderDecoder::statusFrame(vector<string> wordsVector) {
    string frame;
    frame+="SEND\n";
    frame+="destination:"+wordsVector.at(1)+"\n";
    frame+="\n";
    frame+= "book status\n";

    return frame;
}

string StompEncoderDecoder::logoutFrame(vector<string> wordsVector) {
    string frame;
    frame+="DISCONNECT\n";
    frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
    frame+="\n";

    return frame;
}

void StompEncoderDecoder::splitSentence(const string& msg, std::vector<string> &out) {
    size_t start;
    size_t end = 0;
    while ((start = msg.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = msg.find(' ', start);
        out.push_back(msg.substr(start, end - start));
    }
}



