#include <connectionHandler.h>
#include <UserData.h>

UserData* UserData::instance = nullptr;

void UserData::initiate(string &name, string& host, short port) {
    if (instance == nullptr)
    {
        instance = new UserData(name, host, port);
    }
}

UserData* UserData::getInstance() {
    return instance;
}

UserData::UserData(string& name, string& host, short port):
    subscription_id_counter(0), receipt_id_counter(0) ,my_name(name), borrow_map(), inventory(), subscription_map() ,
    host_(host),port_(port), borrow_mtx(), inventory_mtx(), sub_mtx(){}

string& UserData::getName() {
    return my_name;
}

string& UserData::getLender(string& bookName) {
    std::unique_lock<mutex> lck (borrow_mtx);
    return borrow_map.at(bookName);
}

int UserData::generateReceiptID() {
    int id=receipt_id_counter;
    receipt_id_counter++;
    return id;
}

int UserData::generateSubID() {
    int id=subscription_id_counter;
    subscription_id_counter++;
    return id;
}

void UserData::addBook(string &genre, string &bookName) {
    std::unique_lock<mutex> lck (inventory_mtx);
    inventory[genre].push_back(bookName);
}

void UserData::remove(string &genre, string &bookName) {
    std::unique_lock<mutex> lck (inventory_mtx);
    std::vector<std::string>::iterator itr = std::find(inventory[genre].begin(), inventory[genre].end(), bookName);
    if (itr != inventory[genre].cend()) {
        inventory[genre].erase(itr);
    }
}

bool UserData::isExists(const string& genre, string &bookName) {
    std::unique_lock<mutex> lck (inventory_mtx);
    vector<string>::iterator it;
    it = std::find (inventory[genre].begin(), inventory[genre].end(), bookName);
    if (it != inventory[genre].end())
    {
        return true;
    }
    return false;
}

string UserData::getBooks() {
    std::unique_lock<mutex> lck (inventory_mtx);
    string books;
    for(const auto& genre: inventory){
        for(const string& book: genre.second){
            books+=book+",";
        }
    }
    if(books.length()>0)
        books=books.substr(0,books.length()-1);
    return books;
}

short UserData::getPort() { return port_; }
string& UserData::getHost() { return host_; }

void UserData::addSubscription(const string& genre, int id) {
    std::unique_lock<mutex> lck (sub_mtx);
    subscription_map.insert({genre,id});
}

int UserData::getSubByGenre(const string& genre) {
    std::unique_lock<mutex> lck (sub_mtx);
    return subscription_map[genre];
}
