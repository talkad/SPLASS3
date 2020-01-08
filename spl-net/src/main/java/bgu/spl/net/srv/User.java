package bgu.spl.net.srv;

public class User {
    private static int id_counter=0;
    private String name;
    private String pwd;
    private boolean logged_in;
    private int id;

    public User(String name, String pwd) {
        this.name = name;
        this.pwd = pwd;
        this.id=id_counter;
        id_counter++;
        this.logged_in = false;
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
