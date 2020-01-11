#include <Frame.h>
#include "StompMessagingProtocol.h"

string StompMessagingProtocol::process(Frame frame) {
    string command=frame.getCommand();
    string message_body=frame.getMessageBody();
    std::unordered_map<string,string> header_map=frame.getHeaderMap();
    if(command.compare("CONNECTED")==0){
        printf("Login successful \n");
        //login=true
    }
    if(command.compare("ERROR")==0 && command.compare("RECEIPT")==0){
        printf("%s",message_body.c_str());
    }
    if(command.compare("MESSAGE")==0){
        //SEND username has book name
        //
        //sends SEND frame with: taking {bookname} from {bookOwnerUsername}.
        //
    }

    //process of keyboard
    //add and return doesnt need frame
    if(command.compare("SEND")==0){
        //should add the book to the vector
    }

}
