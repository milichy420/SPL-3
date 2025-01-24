package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;

public class StompProtocol implements StompMessagingProtocol<StompFrame>{

    private boolean shouldTerminate = false;
    private int connectionId;
    private Connections<StompFrame> connections;

    @Override
    public void start(int connectionId, Connections<StompFrame> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public StompFrame process(StompFrame msg) {
        switch (msg.getName()) {
            case "CONNECT":
                
                break;
            case "SEND":
                
                break;
            case "SUBSCRIBE":
                
                break;
            case "UNSUBSCRIBE":
                
                break;
            case "DISCONNECT":    
                shouldTerminate = true;
                break;
            default:
                // return the resulting frame here
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }



}
