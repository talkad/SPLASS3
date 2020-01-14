package bgu.spl.net.srv;



import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.concurrent.atomic.AtomicInteger;

public class ConnectionsImpl implements Connections<String> {

    private ConcurrentHashMap<Integer, ConnectionHandler> handlersMap;//connectionid to handler
    private AtomicInteger messageIdCounter;


    public ConnectionsImpl() {
        handlersMap = new ConcurrentHashMap<>();
        messageIdCounter=new AtomicInteger(0);
    }

    @Override
    public boolean send(int connectionId, String msg) {

        if (handlersMap.containsKey(connectionId))
            handlersMap.get(connectionId).send(msg);
        else
            return false;
        return true;
    }

    @Override
    public void send(String channel, String msg) {
        ConcurrentSkipListSet<Integer> idSet = GenreHandler.getInstance().getSubsOfGenre(channel);
        for (Integer id : idSet) {
            Frame frame=new Frame(msg);
            frame.addHeader("Message-id:",""+ messageIdCounter.addAndGet(1));
            frame.addHeader("subscription",""+ GenreHandler.getInstance().getSubscriptionId(id,channel));
            String withIds=frame.toString();
            send(id, withIds);
        }
    }

    @Override
    public void disconnect(int connectionId) {
        try {
            handlersMap.get(connectionId).close();
        } catch (IOException ignored) {
        }
        handlersMap.remove(connectionId);

    }
    public void addConnection(int connectionId, ConnectionHandler handler){
        handlersMap.put(connectionId,handler);
    }


}
