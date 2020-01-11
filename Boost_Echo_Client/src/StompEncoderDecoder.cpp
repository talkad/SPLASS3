//
// Created by amir on 07/01/2020.
//

#include "StompEncoderDecoder.h"
#include <unordered_map>


string StompEncoderDecoder::decodeMessage(string msg) {

}


string StompEncoderDecoder::toStompFrame(string msg) {
    string frame="";
    std::vector<string> wordsVector;
    splitSentence(msg, wordsVector);
    string command=wordsVector.at(0);

    if(command.compare("login")==0){
        frame+="CONNECT\n";
        frame+="accept-version: 1.2\n";
        frame+="host:"+wordsVector.at(1)+"\n";
        frame+="login:"+wordsVector.at(2)+"\n";
        UserData::initiate(wordsVector.at(2));
        frame+="passcode:"+wordsVector.at(3)+"\n";
        frame+="\n";
        frame+="^@";
    }
    if(command.compare("join")==0){
        frame+="SUBSCRIBE\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="id:"+ std::to_string(UserData::getInstance()->generateSubID())+"\n";
        frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
        frame+="\n";
        frame+="^@";
    }
    if(command.compare("exit")==0){
        frame+="UNSUBSCRIBE\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="id:"+ std::to_string(UserData::getInstance()->generateSubID())+"\n";
        frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
        frame+="\n";
        frame+="^@";
    }
    if(command.compare("add")==0){
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+= UserData::getInstance()->getName() +" has added the book "+ wordsVector.at(2)+"\n";
        frame+="^@";
    }



    if(command.compare("borrow")==0){//there is a pingpong with server and we send another message of "taking book"
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+= UserData::getInstance()->getName() +" wish to borrow "+wordsVector.at(2);
        frame+="\n";
        frame+="^@";
    }



    if(command.compare("return")==0){
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+="Returning "+ wordsVector.at(2) + " to " + UserData::getInstance()->getLender(wordsVector.at(2))+"\n";
        frame+="^@";
    }



    if(command.compare("status")==0){ //there is a pingpong with server and we send another message of "taking book"
        frame+="SEND\n";
        frame+="destination:"+wordsVector.at(1)+"\n";
        frame+="\n";
        frame+= "book status\n";
        frame+="^@";
    }


    if(command.compare("logout")==0){
        frame+="DISCONNECT\n";
        frame+="receipt:"+std::to_string(UserData::getInstance()->generateReceiptID())+"\n";
        frame+="\n";
        frame+="^@";
    }

}

void StompEncoderDecoder::splitSentence(string msg, std::vector<string> &out) {
    size_t start;
    size_t end = 0;
    while ((start = msg.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = msg.find(' ', start);
        out.push_back(msg.substr(start, end - start));
    }
}



