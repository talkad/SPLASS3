package bgu.spl.net.srv;

import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class BookClub {

    //doubt if we need to use the implementation of the newsfeed
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<String>> channels = new ConcurrentHashMap<>();
    //all users
    private ConcurrentHashMap<String,User> users; //the key will be the name of the client

    private static class SingletonHolder {
        private static BookClub instance=new BookClub();
    }

     // Retrieves the single instance of this class.
    public static BookClub getInstance() {
        return SingletonHolder.instance;
    }

    public boolean isUsernameExists(String name){
        return users.containsKey(name);
    }

    public boolean isUserConnected(String name){
        User user=users.get(name);
        if(user==null)
            return false;
        return user.isLogged_in();
    }

    public boolean confirmPwd(String name,String pwd){
        User user=users.get(name);
        if(user==null)
            return false;
        return user.getPwd().equals((pwd));
    }

    public void createNewUser(String name,String pwd){
        users.putIfAbsent(name,new User(name,pwd));
    }

    public User getUserByName(String name){
        return users.get(name);
    }

    public User getUserByID(int id){
        for(String name:users.keySet()){
            if(users.get(name).getId()==id)
                return users.get(name);
        }
        return null;
    }



    //irrelevant at this point...
    public ArrayList<String> fetch(String channel) {
        ConcurrentLinkedQueue<String> queue = channels.get(channel);
        if (queue == null) {
            return new ArrayList<>(0); //empty
        } else {
            return new ArrayList<>(queue); //copy of the queue, arraylist is serializable
        }
    }

    public void publish(String channel, String news) {
        ConcurrentLinkedQueue<String> queue = channels.computeIfAbsent(channel, k -> new ConcurrentLinkedQueue<>());
        queue.add(news);
    }

    public void clear() {
        channels.clear();
    }
}
