package bgu.spl.net.srv;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;

public class User {
    private String username;
    private String password;
    private int connectionId;
    private ConcurrentHashMap<Integer, String> subscribedTopics;

    public User(String username, String password, int connectionId){
        this.username = username;
        this.password = password;
        this.connectionId = connectionId;
        subscribedTopics = new ConcurrentHashMap<>();
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public ConcurrentHashMap<Integer, String> getSubscribedTopics() {
        return subscribedTopics;
    }

    public void subscribeToTopic(String topic, int subscriptionId){
        subscribedTopics.put(subscriptionId, topic);
    }

    public void removeSubscribedTopic(int subscriptionId){
        subscribedTopics.remove(subscriptionId);
    }

    public boolean isSubscribedToTopic(String topic){
        return subscribedTopics.contains(topic);
    }

    public ArrayList<Integer> getSubscribedTopicsIds(){
        return new ArrayList<>(subscribedTopics.keySet());
    }

    public void unsuscribeFromAllTopics(){
        subscribedTopics.clear();
    }

    public int getSubscriptionIdForTopic(String topic){
        for (int subscriptionId : subscribedTopics.keySet()){
            if (subscribedTopics.get(subscriptionId).equals(topic)){
                return subscriptionId;
            }
        }
        return -1;
    }


}
