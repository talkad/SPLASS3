#include "StompEncoderDecoder.h"
#include <unordered_map>


string StompEncoderDecoder::toStompFrame(const string& msg) {
    string frame;
    std::vector<string> wordsVector;
    splitSentence(msg, wordsVector);
    string command=wordsVector.at(0);

    if(command=="login"){
        frame+="CONNECT\n";
        frame+="accept-version: 1.2\n";
        frame+="host:"+wordsVector.at(1)+"\n";
        frame+="login:"+wordsVector.at(2)+"\n";
        UserData::initiate(wordsVector.at(2));
        frame+="passcode:"+wordsVector.at(3)+"\n";
        frame+="\n";
        frame+="^@";
    }
    else if(command=="join"){
        frame+="SUBSCRIBE\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="id:"+ std::to_string(UserData::getInstance()->generateSubID())+"\n";
        frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
        frame+="\n";
        frame+="^@";
    }
    else if(command=="exit"){
        frame+="UNSUBSCRIBE\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="id:"+ std::to_string(UserData::getInstance()->generateSubID())+"\n";
        frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
        frame+="\n";
        frame+="^@";
    }
    else if(command=="add"){
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+= UserData::getInstance()->getName() +" has added the book "+ wordsVector.at(2)+"\n";
        UserData::getInstance()->addBook(wordsVector.at(1),wordsVector.at(2));
        frame+="^@";
    }
    else if(command=="borrow"){//the response is handle in the protocol
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+= UserData::getInstance()->getName() +" wish to borrow "+wordsVector.at(2);
        frame+="\n";
        frame+="^@";
    }
    else if(command=="return"){
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+="Returning "+ wordsVector.at(2) + " to " + UserData::getInstance()->getLender(wordsVector.at(2))+"\n";
        UserData::getInstance()->remove(wordsVector.at(1), wordsVector.at(2));
        frame+="^@";
    }
    else if(command=="status"){ //the response is handle in the protocol
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+= "book status\n";
        frame+="^@";
    }
    else if(command=="logout"){
        frame+="DISCONNECT\n";
        frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
        frame+="\n";
        frame+="^@";
    }
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



