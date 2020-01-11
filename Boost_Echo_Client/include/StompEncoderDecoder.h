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
    string toStompFrame(const string& msg);
    static void splitSentence(const string& msg, std::vector<string>& out);

};


#endif //BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
