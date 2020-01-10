package bgu.spl.net.srv;



import java.awt.*;
import java.io.IOException;
import java.util.Hashtable;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;

public class ConnectionsImpl implements Connections<String> {

    private ConcurrentHashMap<Integer, ConnectionHandler> handlersMap;//connectionid to handler
    private ConcurrentHashMap<String, ConcurrentSkipListSet<Integer>> subscribersMap;// key- genrename, element- set of client!! ids
    private ConcurrentHashMap<Integer, ConcurrentHashMap<Integer, String>> subIdClientId_genre;// key- clientid, map that: key-subscriptionid, element-genrename


    //look at BookClub (singleton)
    //i defined these data structure there, you dont need to define them here
    // i am not going to touch your code- do whatever seems right to you
    // good luck mate!
    // you probably wonder where my english from ?
    //from the bathroom
    //private Hashtable<Integer,ConnectionHandler> handlersMap;
    //private Hashtable<String, ConcurrentSkipListSet<Integer>> subscribersMap;// key- genrename, element- set of subs ids


    public ConnectionsImpl() {
        handlersMap = new ConcurrentHashMap<>();
        subscribersMap = new ConcurrentHashMap<>();
        subIdClientId_genre=new ConcurrentHashMap<>();
    }

    @Override
    public boolean send(int connectionId, String msg) {
        handlersMap.get(connectionId).send(msg);
        return true;// just for the hell of it
    }

    @Override
    public void send(String channel, String msg) {
        ConcurrentSkipListSet<Integer> idSet = subscribersMap.get(channel);
        for (Integer id : idSet)
            send(id, msg);
    }

    @Override
    public void disconnect(int connectionId) {
        try {
            handlersMap.get(connectionId).close();
        } catch (IOException ignored) {
        }
        handlersMap.remove(connectionId);
    }

    public void addConnection(int connectionId, ConnectionHandler handler) {
        handlersMap.put(connectionId, handler);
    }

    public void addSubscription(String channel, int clientId, int subscriptionId){//not thread safe
        subscribersMap.putIfAbsent(channel,new ConcurrentSkipListSet<>());
        subscribersMap.get(channel).add(clientId);
        subIdClientId_genre.putIfAbsent(clientId,new ConcurrentHashMap<>());
        subIdClientId_genre.get(clientId).putIfAbsent(subscriptionId,channel);

    }
    public void removeSubscription(int clientId, int subscriptionId){
        String channel = subIdClientId_genre.get(clientId).get(subscriptionId);
        subIdClientId_genre.get(clientId).remove(subscriptionId);
        subscribersMap.get(channel).remove(clientId);

    }



}
