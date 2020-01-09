package bgu.spl.net.srv;

import java.sql.Connection;
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
    public void process(String message) {
        Frame frame=new Frame(message);
        if(frame.getCommand().equals("CONNECT")){
            connectFrameResponse(frame);
        }
        else if(frame.getCommand().equals("SUBSCRIBE")){
            subscribeFrameResponse(frame);
        }
        else if(frame.getCommand().equals("SEND")){
            sendFrameResponse(frame);
        }
        else if(frame.getCommand().equals("DISCONNECT")){
            disconnectFrameResponse(frame);
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
        if(body.contains("has added the book")){ //add command
            addBook(genre,frame.getBody());
        }
        if(body.contains("wish to borrow")){ //borrow command

        }
        if(body.contains("Returning")){ //return command

        }
        if(body.contains("book status")){ //status command

        }
    }

    private void addBook(String genre, String body) {
        String bookName = body.substring(body.lastIndexOf(' ') + 1);
        String clientName = body.substring(0,body.indexOf(' '));
        BookClub.getInstance().getUserByName(clientName).addBook(genre,bookName);
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
                    connected.addHeader("receipt-id",BookClub.getInstance().getUserByName(name).getId()+"");
                    connection.send(connectionId,connected.toString());
                }
                else{ //this client was connected already
                    Frame error=new Frame("ERROR","User already logged in");
                    System.out.println("User already logged in");
                    error.addHeader("message","User already logged in");
                    connection.send(connectionId,error.toString());
                }
            }
            else{ //user exists but wrong pwd
                Frame error=new Frame("ERROR","Wrong password");
                System.out.println("Wrong password");
                error.addHeader("message","Wrong password");
                connection.send(connectionId,error.toString());
            }
        }
        else{//new user login
            BookClub.getInstance().createNewUser(name,pwd);
            BookClub.getInstance().getUserByName(name).login();
            Frame connected=new Frame("CONNECTED","login successful");
            connected.addHeader("version","1.2");
            connected.addHeader("receipt-id",BookClub.getInstance().getUserByName(name).getId()+"");
            connection.send(connectionId,connected.toString());
        }
    }
}
