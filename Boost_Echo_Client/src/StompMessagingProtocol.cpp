#include <Frame.h>
#include <UserData.h>
#include "StompMessagingProtocol.h"




//borrow- the book will be removed from this client if the book exists, but the borrower will not get the book
string StompMessagingProtocol::process(Frame frame) {
    string message="";
    string command=frame.getCommand();
    string message_body=frame.getMessageBody();
    std::unordered_map<string,string> header_map=frame.getHeaderMap();
    if(command=="CONNECTED"){
        printf("Login successful \n");
    }

    else if(command=="ERROR"){
        printf("%s \n",message_body.c_str());
    }

    else if(command.compare("MESSAGE")==0){
        printf("%s \n",frame.toString().c_str());
        string genre=header_map["destination"];
        vector<string> body;
        splitSentence(message_body,body);

        if(message_body.find(" wish to borrow ") != string::npos){
            string bookName="";
            for(int i=4;i<body.size();i++){
                bookName+=body[i];
                if(i+1<body.size())
                    bookName+=" ";
            }

            if(UserData::getInstance()->isExists(genre,bookName)){
                message+="SEND\n";
                message+="destination:"+genre+"\n";
                message+="\n";
                message+= "Taking "+bookName+" from "+UserData::getInstance()->getName() ;
                message+="\n";
                message+="^@";
                UserData::getInstance()->remove(genre,bookName);
            }
        }

        else if(message_body.find("Returning ") != string::npos && message_body.find(" to ") != string::npos){
            string name=body[body.size()-1];
            if(UserData::getInstance()->getName()==name) {
                string bookName = "";
                for (int i = 1; i < body.size() - 2; i++) {
                    bookName += body[i];
                    if (i + 1 < body.size())
                        bookName += " ";
                }
                UserData::getInstance()->addBook(genre,bookName);
            }
        }

        else if(message_body.find("book status") != string::npos){
            string inventory=UserData::getInstance()->getName()+":";
            inventory+=UserData::getInstance()->getBooks();
            message+="SEND\n";
            message+="destination:"+genre+"\n";
            message+="\n";
            message+= inventory ;
            message+="^@";
        }
    }
    return message;
}


void StompMessagingProtocol::splitSentence(const string& msg, std::vector<string> &out) {
    size_t start;
    size_t end = 0;
    while ((start = msg.find_first_not_of(' ', end)) != std::string::npos)
    {
        end = msg.find(' ', start);
        out.push_back(msg.substr(start, end - start));
    }
}
