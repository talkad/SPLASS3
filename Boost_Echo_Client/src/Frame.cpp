//
// Created by amir on 08/01/2020.
//

#include "Frame.h"
using std::vector;
const std::unordered_map<string, string> &Frame::getHeaderMap() const {
    return header_map;
}

const string &Frame::getCommand() const {
    return command;
}

const string &Frame::getMessageBody() const {
    return message_body;
}

void Frame::splitSentence(string msg, std::vector<string> &out,char delimiter) {
    size_t start;
    size_t end = 0;
    while ((start = msg.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = msg.find(delimiter, start);
        out.push_back(msg.substr(start, end - start));
    }
}

const string Frame::toString() {
    string result=command+"\n";
    for (auto x:header_map)
        result+=x.first +":"+x.second+"\n";
    result+="\n";
    result+=message_body;//has null inside already
    return result;
}

Frame::Frame(string msg) {
    vector<string> lines;
    splitSentence(msg,lines,'\n');
    command=lines.at(0);

    for(string s: lines){
        if(size_t k=s.find(':')!=std::string::npos) {
            string header = s.substr(0, k - 1);
            string value = s.substr(k+1,s.length()-k);
            header_map.insert(header,value);
        }
    }

    size_t body_index=msg.find("\n\n");
    message_body=msg.substr(body_index+2);
}

