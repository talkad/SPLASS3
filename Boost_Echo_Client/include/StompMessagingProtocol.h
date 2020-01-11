//
// Created by amir on 08/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
#define BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H

#include <string>
#include <vector>
#include <Frame.h>

using std::string;
using std::vector;
class StompMessagingProtocol {
public:
    string process(Frame& msg);
    void splitSentence(const string &msg, vector<string> &out);
};


#endif //BOOST_ECHO_CLIENT_STOMPMESSAGINGPROTOCOL_H
