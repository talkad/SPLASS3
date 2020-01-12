//
// Created by amir on 08/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H

#include <string>
#include <vector>

using std::string;
using std::vector;
class StompMessagingProtocol {
public:
    string process(string& msg);
    static void splitSentence(const string& msg, std::vector<string> &out,char delimiter);
    int indexOf(string& text, string& pattern);
};


#endif //BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
