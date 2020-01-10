package bgu.spl.net.srv;

import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class BookClub {

    //the key will be the genre
    //each genre preserve the id of the clients who related to it
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<Integer>> genre_map = new ConcurrentHashMap<>();
    //all users
    private ConcurrentHashMap<String,User> users= new ConcurrentHashMap<>(); //the key will be the name of the client
    private static int messageID=0;


    private static class SingletonHolder {
        private static BookClub instance=new BookClub();
    }

     // Retrieves the single instance of this class.
    public static BookClub getInstance() {
        return SingletonHolder.instance;
    }

    public String generateMsgID(){
        String id=messageID+"";
        while(id.length()<5){
            id="0"+id;
        }
        messageID++;
        return id;
    }

    public ConcurrentLinkedQueue<Integer> getSubsByGenre(String genre){
        return genre_map.get(genre);
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

    public void removeClient(int id){
        for(String genre:genre_map.keySet()){
            genre_map.get(genre).remove(id);
        }
    }

    public void subscribeGenre(String genre,int id){
        ConcurrentLinkedQueue<Integer> usersID = genre_map.computeIfAbsent(genre, k -> new ConcurrentLinkedQueue<>());
        usersID.add(id);
    }

    public void unsubscribeGenre(String genre, int connectionId) {
        genre_map.get(genre).remove(connectionId);
    }

    public void addGenreToUser(int userID, String genre, String genreID) {
        getUserByID(userID).addGenre(Integer.parseInt(genreID),genre);
    }

//    //irrelevant at this point...
//    public ArrayList<String> fetch(String channel) {
//        ConcurrentLinkedQueue<String> queue = channels.get(channel);
//        if (queue == null) {
//            return new ArrayList<>(0); //empty
//        } else {
//            return new ArrayList<>(queue); //copy of the queue, arraylist is serializable
//        }
//    }
//
//    public void publish(String channel, String news) {
//        ConcurrentLinkedQueue<String> queue = channels.computeIfAbsent(channel, k -> new ConcurrentLinkedQueue<>());
//        queue.add(news);
//    }
//
//    public void clear() {
//        channels.clear();
//    }
}
