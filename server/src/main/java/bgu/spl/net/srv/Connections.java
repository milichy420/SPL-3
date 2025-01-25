package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    String login(String username, String password, int connectionId);

    void disconnect(int connectionId);
}
