package bgu.spl.net.srv;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Collections;
import java.util.HashMap;
import java.util.concurrent.atomic.*;

import bgu.spl.net.impl.stomp.StompFrame;

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

    public void addConnection(int connectionId, ConnectionHandler<T> handler){
        connections.put(connectionId, handler);
    }

    public int getAndIncrementMessageId(){
        return currentMessageID.getAndIncrement();
    }
    public boolean subscribeToTopic(String topic, int subscriptionId, int connectionId){
        // check if user is already subscribed to topic and if so return false
        if (active_logins.get(connections.get(connectionId).getUser().getUsername()).getSubscribedTopics().containsValue(topic)){
            return false;
        }

        if(!topics.containsKey(topic)){
            topics.put(topic, new ArrayList<>());
        }
        topics.get(topic).add(connectionId);
        active_logins.get(connections.get(connectionId).getUser().getUsername()).subscribeToTopic(topic, subscriptionId);
        return true;
    }

    public boolean unsubscribeFromTopic(int subscriptionId, int connectionId){
        User user = connections.get(connectionId).getUser();
        String topic = user.getSubscribedTopics().get(subscriptionId);
        // check if user is subscribed to topic and if not return false
        if (topic == null){
            return false;
        }

        topics.get(topic).remove(connectionId);
        user.removeSubscribedTopic(subscriptionId);
        return true;
    }


    public String login(String username, String password, int connectionId){
        // check if user is already logged in
        if(active_logins.containsKey(username)){
            return "already logged in";
        }

        // check if user is new
        User user = inactive_logins.get(username);
        if(user == null){
            loginNewUser(username, password, connectionId);
            return "connected";
        }

        // check if password is correct for existing user
        if(user.getPassword().equals(password)){
            loginExistingUser(username, connectionId);
            return "connected";
        }
        return username;
    }

    public void loginNewUser(String username, String password, int connectionId){
        User user = new User(username, password, connectionId);
        active_logins.put(username, user);
        connections.get(connectionId).setUser(user);
    }

    public void loginExistingUser(String username, int connectionId){
        User user = inactive_logins.get(username);
        active_logins.put(username, user);
        inactive_logins.remove(username);
        connections.get(connectionId).setUser(user);
    }

    @Override
    public boolean send(int connectionId, T msg) {
        connections.get(connectionId).send(msg);
        return true;
    }

    @Override
    public void send(String channel, T msg) {
        User user;
        int subscriptionId;
        for (Integer connectionId : topics.get(channel)) {
            user = connections.get(connectionId).getUser();
            subscriptionId = user.getSubscriptionIdForTopic(channel);
            send(connectionId, connections.get(connectionId).getProtocol().addSubscriptionIdToMessage(msg, subscriptionId));
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
            topics.get(topic).remove(connectionId);
        }

        // remove user from active logins, connections and unsubscribe from all topics
        connections.remove(connectionId);
        user.unsuscribeFromAllTopics();
        active_logins.remove(user.getUsername());
        inactive_logins.put(user.getUsername(), user);
    }

    @Override
    public boolean isUserSubscribedToTopic(int connectionId, String topic) {
        return active_logins.get(connections.get(connectionId).getUser().getUsername()).getSubscribedTopics().containsValue(topic);
    }

}
