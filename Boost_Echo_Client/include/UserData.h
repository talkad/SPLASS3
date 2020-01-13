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
    static void initiate(string& name,string& host, short port);
    static UserData* getInstance();
    void addBook(string& genre,string& bookName);
    void remove(string& genre,string& bookName);
    int generateSubID();
    int generateReceiptID();
    string& getName();
    string& getLender(string& bookName);
    //check if book exists it inventory
    bool isExists(string basicString, string &bookName);
    string& getBooks();
    string& getHost();
    short getPort();

private:
    UserData(string& name, string& host, short port);
    static UserData* instance;

    int subscription_id_counter;
    int receipt_id_counter;
    string my_name;
    unordered_map<string,string> borrow_map; //key- name of the book, value- the lender
    std:: unordered_map< string, vector<string> >inventory;

    string host_;
    short port_;
};

#endif //BOOST_ECHO_CLIENT_USERDATA_H
