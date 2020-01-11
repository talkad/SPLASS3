#include <connectionHandler.h>
#include <UserData.h>

UserData* UserData::instance = 0;

void UserData::initiate(string &name) { //TODO: delete this pointer at the end of the connection- delete UserData::getInstance()
    if (instance == 0)
    {
        instance = new UserData(name);
    }
}

UserData* UserData::getInstance() {
    return instance;
}

UserData::UserData(string& name): subscription_id_counter(0), receipt_id_counter(0) ,my_name(name){}

string& UserData::getName() {
    return my_name;
}

string& UserData::getLender(string& bookName) {
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
    inventory[genre].push_back(bookName);
}

void UserData::remove(string &genre, string &bookName) {
    std::vector<std::string>::iterator itr = std::find(inventory[genre].begin(), inventory[genre].end(), bookName);
    if (itr != inventory[genre].cend()) {
        inventory[genre].erase(itr);
    }

}
