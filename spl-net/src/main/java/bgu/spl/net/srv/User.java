package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class User {
    private static int id_counter=0;
    private String name;
    private String pwd;
    private boolean logged_in;
    private int id;
    private ConcurrentHashMap<Integer,String> genre_map;//map for every id its genre name
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<String>> inventory;

    public User(String name, String pwd) {
        this.name = name;
        this.pwd = pwd;
        this.id=id_counter;
        id_counter++;
        this.logged_in = false;
        genre_map=new ConcurrentHashMap<>();
        inventory=new ConcurrentHashMap<>();
    }

    public void addBook(String genre,String bookName){
        ConcurrentLinkedQueue<String> usersID = inventory.computeIfAbsent(genre, k -> new ConcurrentLinkedQueue<>());
        usersID.add(bookName);
    }

    public void addGenre(int id,String genre){
        genre_map.put(id,genre);
    }

    public String getName() {
        return name;
    }

    public String getPwd() {
        return pwd;
    }

    public int getId() { return id; }

    public boolean isLogged_in() {
        return logged_in;
    }

    public void login(){
        logged_in=true;
    }

    public void logout(){
        logged_in=false;
    }
}
