//
// Created by amir on 08/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_FRAME_H
#define BOOST_ECHO_CLIENT_FRAME_H


#include <unordered_map>
#include <vector>

using std::string;
class Frame {
public:
    const std::unordered_map<string, string> &getHeaderMap() const;
    const string &getCommand() const;
    const string &getMessageBody() const;
    Frame(string);
    const string toString();
    void splitSentence(string msg, std::vector<string> &out,char delimiter);

private:
    std::unordered_map<string,string> header_map;
    string command;
    string message_body;
};


#endif //BOOST_ECHO_CLIENT_FRAME_H
