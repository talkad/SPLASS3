package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    void addConnection(int connectionId, ConnectionHandler handler);

    void addSubscription(String channel, int clientId, int subscriptionId);

    void removeSubscription(int connectionId, int subscriptionId);
}
