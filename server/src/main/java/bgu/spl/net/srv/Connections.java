package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    String login(String username, String password, int connectionId);

    boolean subscribeToTopic(String topic, int subscriptionId, int connectionId);

    boolean unsubscribeFromTopic(int subscriptionId, int connectionId);

    boolean isUserSubscribedToTopic(int connectionId, String topic);

    int getAndIncrementMessageId();
    
    void disconnect(int connectionId);
}
