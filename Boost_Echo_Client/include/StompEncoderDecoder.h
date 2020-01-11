//
// Created by amir on 07/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
#define BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
#include <string>
#include <vector>
#include <unordered_map>
#include "UserData.h"

using std::string;
using std::unordered_map;

class StompEncoderDecoder {
public :
    string encodeFrame(string frame);
    string toStompFrame(string msg);
    string decodeMessage(string msg);
    void splitSentence(string msg, std::vector<string>& out);


private:
};


#endif //BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
