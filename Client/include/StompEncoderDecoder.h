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

    string loginFrame(vector<string> wordsVector);
    string joinFrame(vector<string> wordsVector);
    string exitFrame(vector<string> wordsVector);
    string addFrame(vector<string> wordsVector);
    string borrowFrame(vector<string> wordsVector);
    string returnFrame(vector<string> wordsVector);
    string statusFrame(vector<string> wordsVector);
    string logoutFrame(vector<string> wordsVector);

    static void splitSentence(const string& msg, std::vector<string>& out);
    int indexOf(string &text, string &pattern);
};


#endif //BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
