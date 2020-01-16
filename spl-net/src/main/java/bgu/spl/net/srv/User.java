package bgu.spl.net.srv;

import java.util.List;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.ConcurrentSkipListSet;

public class User {

    private static class SingletonHolder {
        private static User instance=new User();
    }
    private ConcurrentHashMap<String,Boolean> isConnectedMap=new ConcurrentHashMap<>();// name to connected.
    private ConcurrentHashMap<String,String> isGoodPwd=new ConcurrentHashMap<>();// name to password;
    private ConcurrentHashMap<Integer,String> connectionIdToName=new ConcurrentHashMap<>();
    private ConcurrentSkipListSet<Integer> connectedIds=new ConcurrentSkipListSet<>();//DEBUG


    public static User getInstance() {
        return SingletonHolder.instance;
    }
    boolean isConnected(String name){
        return isConnectedMap.get(name);
    }

    boolean isExisting(String name){
        return isGoodPwd.containsKey(name);
    }

    boolean isGoodPwd(String name, String pwd){
        return isGoodPwd.get(name).equals(pwd);
    }

    void login(String name,int connectionId){
        isConnectedMap.replace(name,true);
        connectionIdToName.put(connectionId,name);
        System.out.println(connectionIdToName.get(connectionId) + " is connected");
    }

    void logout(int connectionId){
        isConnectedMap.replace(connectionIdToName.get(connectionId),false);
        System.out.println(connectionIdToName.get(connectionId) + " disconnected");//DEBUG
        connectionIdToName.remove(connectionId);
        GenreHandler.getInstance().disconnect(connectionId);
    }
    void addNewUserConnected(int connectionId,String name, String pwd){
        isGoodPwd.putIfAbsent(name,pwd);
        isConnectedMap.putIfAbsent(name,true);
        connectionIdToName.put(connectionId,name);
    }

}
