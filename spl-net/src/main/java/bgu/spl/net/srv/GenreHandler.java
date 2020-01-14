package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.ConcurrentSkipListSet;

public class GenreHandler {

    private ConcurrentHashMap<String, ConcurrentSkipListSet<Integer>> subscribersMap;// key- genrename, element- set of
    private ConcurrentHashMap<Integer, ConcurrentHashMap<Integer, String>> connectionId_SubId_genre;// key- clientid, map that: key-subscriptionid, element-genrename

    private static int messageID=0;

    private static class SingletonHolder {
        private static GenreHandler instance=new GenreHandler();
    }

     // Retrieves the single instance of this class.
    public static GenreHandler getInstance() {
        return SingletonHolder.instance;
    }



    public ConcurrentSkipListSet<Integer> getSubsOfGenre(String genre){
        return subscribersMap.get(genre);
    }

    public void disconnect(int connectionId){
        for(String genre : subscribersMap.keySet()){
            subscribersMap.get(genre).remove(connectionId);
        }
        connectionId_SubId_genre.remove(connectionId);
    }

    public void subscribeGenre(String genre,int connectionId,int subscriptionId){
        //assumes connectionId is in connectionId_SubId_genre
        subscribersMap.get(genre).add(connectionId);
        connectionId_SubId_genre.get(connectionId).putIfAbsent(subscriptionId,genre);
    }

    public void unsubscribeGenre(int connectionId,int subscriptionId ) {
        String genre=connectionId_SubId_genre.get(connectionId).get(subscriptionId);
        subscribersMap.get(genre).remove(connectionId);
        connectionId_SubId_genre.remove(connectionId).get(subscriptionId);
    }
    public int getSubscriptionId(int connectionId,String channel){
        for(int key:connectionId_SubId_genre.get(connectionId).keySet())
            if(connectionId_SubId_genre.get(connectionId).get(key).equals(channel))
                return key;
        return -1;
    }
}
