//
// Created by amir on 08/01/2020.
//

#include <vector>
#include <unordered_map>

#ifndef BOOST_ECHO_CLIENT_CLIENT_H
#define BOOST_ECHO_CLIENT_CLIENT_H

#endif //BOOST_ECHO_CLIENT_CLIENT_H
using std::vector;
using std::string;
using std::vector;
using std::string;

class Client{
private:
    bool login=false;
    int subscription_id_counter=0;
    int receipt_id_counter=0;
    string my_name;
    std:: unordered_map<string,int> borrow_map;
    std:: unordered_map< string, vector<string> >books_of_genre;
};
