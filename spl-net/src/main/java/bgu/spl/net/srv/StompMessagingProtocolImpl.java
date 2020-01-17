package bgu.spl.net.srv;

import java.util.Hashtable;

public class StompMessagingProtocolImpl implements bgu.spl.net.api.StompMessagingProtocol {
    //client’s connection id.
    private int connectionId;
    //client's connection.
    private ConnectionsImpl connections;
    private boolean shouldTerminate=false;

    @Override
    public void start(int connectionId, ConnectionsImpl connections) {
        this.connectionId=connectionId;
        this.connections=connections;
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
        sendMessage(body,genre);//sends the massage with the same body to everybody
    }

    private void sendMessage(String body, String genre) {
        //message id and subscriptionId are made in connections
        Frame frame=new Frame("MESSAGE",body);
        frame.addHeader("destination",genre);
        connections.send(genre,frame.toString());
    }

    //this function handles join genre- SUBSCRIBE frame
    private void subscribeFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String receiptID=headers.get("receipt");
        String subID=headers.get("id");
        String genre=headers.get("destination");
        //add the client to a reading club
        GenreHandler.getInstance().subscribeGenre(genre,connectionId,Integer.parseInt(subID));
        Frame receipt=new Frame("RECEIPT","Joined club "+genre);
        receipt.addHeader("receipt",receiptID);
        connections.send(connectionId,receipt.toString());
    }

    //this function handles exit genre- UNSUBSCRIBE frame
    private void unsubscribeFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String receiptID=headers.get("receipt");
        String subscriptionId=headers.get("id");
        String genre=headers.get("destination");
        //remove the client to a reading club
        GenreHandler.getInstance().unsubscribeGenre(connectionId,Integer.parseInt(subscriptionId));
        Frame receipt=new Frame("RECEIPT","Exited club "+genre);
        receipt.addHeader("receipt",receiptID);
        connections.send(connectionId,receipt.toString());
    }

    //this function handles logout- DISCONNECT frame
    private void disconnectFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String receiptID=headers.get("receipt");
        Frame receipt=new Frame("RECEIPT","");
        receipt.addHeader("receipt-id",receiptID);
        connections.send(connectionId,receipt.toString());
        //remove the client from all the topics
        GenreHandler.getInstance().disconnect(connectionId);
        User.getInstance().logout(connectionId);
        shouldTerminate=true;
        connections.disconnect(connectionId);
    }

    //this function handles all possible scenarios of login- CONNECT frame
    private void connectFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        String name=headers.get("login");
        String pwd=headers.get("passcode");
        User user=User.getInstance();
        if(user.isExisting(name)){ //the username exists in the server data
            if(user.isGoodPwd(name,pwd)){
                //good password
                if(!user.isConnected(name)){
                    //wasn't logged in
                    user.login(name,connectionId);
                    Frame connected=new Frame("CONNECTED","login successful");
                    connected.addHeader("version","1.2");
                    connections.send(connectionId,connected.toString());
                }
                else{ //this client was connected already
                    Frame error=new Frame("ERROR","User already logged in");
                    error.addHeader("message","User already logged in");
                    connections.send(connectionId,error.toString());
                    GenreHandler.getInstance().disconnect(connectionId);
                    user.logout(connectionId);
                    shouldTerminate=true;
                    connections.disconnect(connectionId);
                }

            }
            else{ //user exists but wrong pwd
                Frame error=new Frame("ERROR","Wrong password");
                error.addHeader("message","Wrong password");
                connections.send(connectionId,error.toString());
                GenreHandler.getInstance().disconnect(connectionId);
                user.logout(connectionId);
                //shouldTerminate=true;
                connections.disconnect(connectionId);
            }
        }
        else{//new user login
            user.addNewUserConnected(connectionId,name,pwd);
            user.login(name,connectionId);
            Frame connected=new Frame("CONNECTED","login successful");
            connected.addHeader("version","1.2");
            connections.send(connectionId,connected.toString());
        }
    }
}

