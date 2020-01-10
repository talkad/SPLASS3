package bgu.spl.net.srv;

import java.util.Hashtable;

public class StompMessagingProtocol implements bgu.spl.net.api.StompMessagingProtocol {
    //client’s connection id.
    private int connectionId;
    //client's connection.
    private Connections connection;
    private boolean shouldTerminate=false;

    @Override
    public void start(int connectionId, Connections<String> connections) {
        this.connectionId=connectionId;
        this.connection=connections;
    }

    @Override
    public void process(Frame frame) {
        switch (frame.getCommand()) {
            case "CONNECT":
                connectFrameResponse(frame);
                break;
            case "SUBSCRIBE":
                subscribeFrameResponse(frame);
                break;
            case "UNSUBSCRIBE":
                unsubscribeFrameResponse(frame);
                break;
            case "SEND":
                sendFrameResponse(frame);
                break;
            case "DISCONNECT":
                disconnectFrameResponse(frame);
                break;
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    //this function handles all possible scenarios of SEND frame
    private void sendFrameResponse(Frame frame) {
        String body=frame.getBody();
        Hashtable<String,String> headers=frame.getHeaders();
        String genre= headers.get("destination");

        if(body.contains("taking ") && body.contains(" from ")){ //can take book from some other client- borrowing scenario
            takeBook(body,genre);
        }
        else{
            sendMessage(body,genre);
        }
    }

    private void takeBook(String body, String genre) {
        String[] msg=body.split(" ");
        String bookName="";
        for(int i=1;i<msg.length-2;i++)
        {
            bookName+=msg[i]+" ";
        }
        sendMessage(msg[msg.length-1]+" has "+bookName,genre);

        sendMessage(body,genre);
    }


    private void sendMessage(String body, String genre) {
        Frame frame=new Frame("MESSAGE",body);
        frame.addHeader("subscription", BookClub.getInstance().getUserByID(connectionId).getGenreID(genre)+"");
        frame.addHeader("Message-id",BookClub.getInstance().generateMsgID());
        frame.addHeader("destination",genre);
        connection.send(genre,frame.toString());
    }

    //this function handles join genre- SUBSCRIBE frame
    private void subscribeFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String receiptID=headers.get("receipt");
        String genreID=headers.get("id");
        String genre=headers.get("destination");
        //add the client to a reading club
        BookClub.getInstance().addGenreToUser(connectionId,genre,genreID);
        BookClub.getInstance().subscribeGenre(genre,connectionId);
        Frame receipt=new Frame("RECEIPT","Joined club "+genre);
        receipt.addHeader("receipt",receiptID);
        connection.send(connectionId,receipt.toString());
    }

    //this function handles exit genre- UNSUBSCRIBE frame
    private void unsubscribeFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String receiptID=headers.get("receipt");
        String genreID=headers.get("id");
        String genre=headers.get("destination");
        //remove the client to a reading club
        BookClub.getInstance().unsubscribeGenre(genre,connectionId);
        Frame receipt=new Frame("RECEIPT","Exited club "+genre);
        receipt.addHeader("receipt",receiptID);
        connection.send(connectionId,receipt.toString());
    }

    //this function handles logout- DISCONNECT frame
    private void disconnectFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String receiptID=headers.get("receipt");
        Frame receipt=new Frame("RECEIPT","");
        receipt.addHeader("receipt-id",receiptID);
        connection.send(connectionId,receipt);
        //remove the client from all the topics
        BookClub.getInstance().removeClient(connectionId);
        BookClub.getInstance().getUserByID(connectionId).logout();
        connection.disconnect(connectionId);
    }

    //this function handles all possible scenarios of login- CONNECT frame
    private void connectFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String name=headers.get("login");
        String pwd=headers.get("passcode");
        if(BookClub.getInstance().isUsernameExists(name)){ //the username exists in the server data
            if(BookClub.getInstance().confirmPwd(name,pwd)){
                if(!BookClub.getInstance().isUserConnected(name)){
                    BookClub.getInstance().getUserByName(name).login();
                    Frame connected=new Frame("CONNECTED","login successful");
                    connected.addHeader("version","1.2");
                    connection.send(connectionId,connected.toString());
                }
                else{ //this client was connected already
                    Frame error=new Frame("ERROR","User already logged in");
                    error.addHeader("message","User already logged in");
                    connection.send(connectionId,error.toString());
                    shouldTerminate=true;
                    connection.disconnect(connectionId);
                }
            }
            else{ //user exists but wrong pwd
                Frame error=new Frame("ERROR","Wrong password");
                error.addHeader("message","Wrong password");
                connection.send(connectionId,error.toString());
                shouldTerminate=true;
                connection.disconnect(connectionId);
            }
        }
        else{//new user login
            BookClub.getInstance().createNewUser(name,pwd);
            BookClub.getInstance().getUserByName(name).login();
            Frame connected=new Frame("CONNECTED","login successful");
            connected.addHeader("version","1.2");
            connection.send(connectionId,connected.toString());
        }
    }
}
