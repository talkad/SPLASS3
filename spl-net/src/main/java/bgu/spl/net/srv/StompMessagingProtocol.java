package bgu.spl.net.srv;

import java.sql.Connection;

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
        
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
