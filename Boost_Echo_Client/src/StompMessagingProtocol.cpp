#include <UserData.h>
#include <StompMessagingProtocol.h>

//borrow- the book will be removed from this client if the book exists, but the borrower will not get the book
string StompMessagingProtocol::process(string& frame) {

    auto* header_map=new unordered_map<string,string>();
    vector<string> lines;
    splitSentence(frame,lines,'\n');
    string command=lines.at(0);

    string colon=":";
    for(size_t i=0;i<lines.size()-2;i++)
    {
        string line=lines[i];
        int pos=indexOf(line,colon);
        if(pos>0)
        {
            string head=line.substr(0,pos-1);
            string body=line.substr(pos+1);
            header_map->insert({head,body});
        }
    }

    size_t body_index=frame.find("\n\n");
    string message_body=frame.substr(body_index+2);


    string message;
    if(command=="CONNECTED"){
        printf("Login successful \n");
    }

    else if(command=="ERROR"){
        printf("%s \n",message_body.c_str());
    }

    else if(command.compare("MESSAGE")==0){
        printf("%s \n",frame.c_str());
        string genre=(*header_map)["destination"];
        vector<string> body;
        splitSentence(message_body,body,' ');

        if(message_body.find(" wish to borrow ") != string::npos){
            string bookName;
            for(size_t i=4;i<body.size();i++){
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
                string bookName;
                for (size_t i = 1; i < body.size() - 2; i++) {
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
    delete header_map;
    return message;
}


void StompMessagingProtocol::splitSentence(const string& msg, std::vector<string> &out,char delimiter) {
    size_t start;
    size_t end = 0;
    while ((start = msg.find_first_not_of(delimiter, end)) != std::string::npos)
    {
        end = msg.find(delimiter, start);
        out.push_back(msg.substr(start, end - start));
    }
}

int StompMessagingProtocol::indexOf(string &text, string &pattern) {
    std::string::size_type loc = text.find(pattern, 0);
    if(loc != std::string::npos)
    {
        return loc;
    }
    else
    {
        return -1;
    }
}