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
        messageIdCounter=new AtomicInteger(1);
    }

    @Override
    public boolean send(int connectionId, String msg) {

        if (handlersMap.containsKey(connectionId)) {
            handlersMap.get(connectionId).send(new Frame(msg));
        }
        else
            return false;
        return true;
    }

    @Override
    public void send(String channel, String msg) {
        ConcurrentSkipListSet<Integer> idSet = GenreHandler.getInstance().getSubsOfGenre(channel);
        if(idSet!=null) {
            for (Integer id : idSet) {
                Frame frame = new Frame(msg);
                int messageId = messageIdCounter.incrementAndGet();
                frame.addHeader("Message-id", Integer.toString(messageId));
                frame.addHeader("subscription", Integer.toString(GenreHandler.getInstance().getSubscriptionId(id, channel)));
                String withIds = frame.toString();
                send(id, withIds);
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        handlersMap.remove(connectionId);
    }
    public void addConnection(int connectionId, ConnectionHandler handler){
        handlersMap.put(connectionId,handler);
    }
}
