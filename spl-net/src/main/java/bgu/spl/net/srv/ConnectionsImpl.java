package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentMap;

public class ConnectionsImpl<T> implements Connections {
    //the key is the client id and the value is its proper connectionHandler
    private ConcurrentMap<Integer,ConnectionHandler<T>> connections_map;

    @Override
    public boolean send(int connectionId, Object msg) {
        return false;
    }

    @Override
    public void send(String channel, Object msg) {

    }

    @Override
    public void disconnect(int connectionId) {
        ConnectionHandler connection=connections_map.get(connectionId);
        try {
            connection.close();
        }catch (Exception e){}
        connections_map.remove(connectionId);
    }
}
