#ifndef BOOST_ECHO_CLIENT_USERDATA_H
#define BOOST_ECHO_CLIENT_USERDATA_H

#include <vector>
#include <unordered_map>
#include <mutex>

using std::vector;
using std::string;
using std::vector;
using std::unordered_map;
using std::mutex;

class UserData{
public:
    static void initiate(string& name,string& host, short port);
    static UserData* getInstance();
    void addBook(const string& genre,const string& bookName);
    void remove(string& genre,string& bookName);
    void removeBorrow(const string& bookName);
    int generateSubID();
    int generateReceiptID();
    string& getName();
    string& getLender(string& bookName);
    void addBorrowedBook(const string& bookName);
    void updateLender(const string& bookName, const string& lenderName,const string& genre);
    //check if book exists it inventory
    bool isExists(const string& basicString, string &bookName);
    string getBooks(const string& genre);
    string& getHost();
    short getPort();
    void addSubscription(const string& genre,int id);
    int getSubByGenre(const string& genre);

private:
    UserData(string& name, string& host, short port);
    static UserData* instance;

    int subscription_id_counter;
    int receipt_id_counter;
    string my_name;
    unordered_map<string,string> borrow_map; //key- name of the book, value- the lender
    unordered_map< string, vector<string> >inventory;
    unordered_map<string, int> subscription_map;

    string host_;
    short port_;

    mutex borrow_mtx;
    mutex inventory_mtx;
    mutex sub_mtx;
};

#endif //BOOST_ECHO_CLIENT_USERDATA_H
