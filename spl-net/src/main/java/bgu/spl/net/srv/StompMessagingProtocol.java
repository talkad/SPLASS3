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
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    //this function handles all possible scenarios of login- CONNECT frame
    private void connectFrameResponse(Frame frame) {
        Hashtable<String,String> headers=frame.getHeaders();
        //if()
    }
}
