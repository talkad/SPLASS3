#ifndef BOOST_ECHO_CLIENT_USERDATA_H
#define BOOST_ECHO_CLIENT_USERDATA_H

#include <vector>
#include <unordered_map>

using std::vector;
using std::string;
using std::vector;
using std::unordered_map;

class UserData{
public:
    static void initiate(string& name);
    static UserData* getInstance();
    void addBook(string& genre,string& bookName);
    void remove(string& genre,string& bookName);
    int generateSubID();
    int generateReceiptID();
    string& getName();
    string& getLender(string& bookName);

private:
    UserData(string& name);
    static UserData* instance;

    int subscription_id_counter;
    int receipt_id_counter;
    string my_name;
    unordered_map<string,string> borrow_map; //key- name of the book, value- the lender
    std:: unordered_map< string, vector<string> >inventory;
};

#endif //BOOST_ECHO_CLIENT_USERDATA_H
