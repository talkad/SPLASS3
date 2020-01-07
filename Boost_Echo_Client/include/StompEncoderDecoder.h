//
// Created by amir on 07/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
#define BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
class StompEncoderDecoder {
public :
    string encodeMessage(string msg);
    string decodeMessage(string msg);
    void splitSentence(string msg, std::vector<string>& out);

private:
    int subscription_id_counter=0;
    int receipt_id_counter=0;
    string my_name;
    std:: unordered_map<string,string> borrow_map;

};


#endif //BOOST_ECHO_CLIENT_STOMPENCODERDECODER_H
