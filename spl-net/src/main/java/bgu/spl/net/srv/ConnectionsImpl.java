package bgu.spl.net.srv;



import java.awt.*;
import java.io.IOException;
import java.util.Hashtable;
import java.util.concurrent.ConcurrentSkipListSet;

public class ConnectionsImpl implements Connections<String> {
    private Hashtable<Integer,ConnectionHandler> handlersMap;
    private Hashtable<String, ConcurrentSkipListSet<Integer>> subscribersMap;// key- genrename, element- set of subs ids


    @Override
    public boolean send(int connectionId, String msg) {
        handlersMap.get(connectionId).send(msg);
        return true;// just for the hell of it
    }

    @Override
    public void send(String channel, String msg) {
        ConcurrentSkipListSet<Integer> idSet=subscribersMap.get(channel);
        for(Integer id: idSet)
            send(id,msg);
    }

    @Override
    public void disconnect(int connectionId) {
        try {
            handlersMap.get(connectionId).close();
        }
        catch (IOException ignored){}
        handlersMap.remove(connectionId);

    }
}
