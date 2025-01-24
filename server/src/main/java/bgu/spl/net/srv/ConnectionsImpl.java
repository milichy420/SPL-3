package bgu.spl.net.srv;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Collections;
import java.util.concurrent.atomic.*;

public class ConnectionsImpl <T> implements Connections <T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connections; 
    private ConcurrentHashMap<String, ArrayList<Integer>> topics;
    private ConcurrentHashMap<String, User> active_logins;
    private ConcurrentHashMap<String, User> inactive_logins;
    private AtomicInteger currentMessageID;

    public ConnectionsImpl(){
        connections = new ConcurrentHashMap<>();
        topics = new ConcurrentHashMap<>();
        active_logins = new ConcurrentHashMap<>();
        inactive_logins = new ConcurrentHashMap<>();
        currentMessageID = new AtomicInteger(0);
    }

    @Override
    public boolean send(int connectionId, T msg) {
        connections.get(connectionId).send(msg);
        return true;
    }

    @Override
    public void send(String channel, T msg) {
        for (Integer connectionId : topics.get(channel)) {
            send(connectionId, msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {
        // get user and subsctiptions
        User user = connections.get(connectionId).getUser();
        ConcurrentHashMap<Integer, String> subscribedTopics = user.getSubscribedTopics();

        // remove user from all topics
        for(int subscriptionId : Collections.list(subscribedTopics.keys())){
            String topic = subscribedTopics.get(subscriptionId);
            topics.get(topic).remove(subscriptionId);
        }

        // remove user from active logins, connections and unsubscribe from all topics
        connections.remove(connectionId);
        user.unsuscribeFromAllTopics();
        active_logins.remove(user.getUsername());
        inactive_logins.put(user.getUsername(), user);
    }

}
