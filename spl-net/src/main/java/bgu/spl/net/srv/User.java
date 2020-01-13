package bgu.spl.net.srv;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class User {

    private String name;
    private String pwd;
    private boolean logged_in;
    private int id;
    private ConcurrentHashMap<String,Integer> genre_map;//map for every id its genre name

    public User(int id,String name, String pwd) {
        this.name = name;
        this.pwd = pwd;
        this.id=id;
        this.logged_in = false;
        genre_map=new ConcurrentHashMap<>();
    }

    public int getGenreID(String genre){
        return genre_map.get(genre);
    }

    public void addGenre(int id,String genre){
        genre_map.put(genre,id);
    }

    public String getPwd() {
        return pwd;
    }

    public int getId() { return id; }

    public void updateId(int newId){id=newId;}

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
