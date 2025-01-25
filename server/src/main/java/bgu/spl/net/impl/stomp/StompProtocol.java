package bgu.spl.net.impl.stomp;

import java.util.HashMap;

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

    public StompFrame addSubscriptionIdToMessage(StompFrame msg, int subscriptionId){
        HashMap<String, String> headers = msg.getHeaders();
        headers.put("subscription", String.valueOf(subscriptionId));
        return new StompFrame(msg.getName(), headers, msg.getBody());
    }

    @Override
    public StompFrame process(StompFrame msg) {
        StompFrame receiptFrame;
        if (msg.getHeaders().containsKey("receipt")) {
            FrameHandler.handleReceipt(msg, connections, connectionId);
        }
        StompFrame outputFrame = null;
        switch (msg.getName()) {
            case "CONNECT":
                outputFrame = FrameHandler.handleConnect(msg, connections, connectionId);
                break;
            case "SEND":
                return FrameHandler.handleSend(msg, connections, connectionId);
            case "SUBSCRIBE":
                return FrameHandler.handleSubscribe(msg, connections, connectionId);
            case "UNSUBSCRIBE":
                return FrameHandler.handleUnsubscribe(msg, connections, connectionId);
            case "DISCONNECT":    
                FrameHandler.handleDisconnect(msg, connections, connectionId);
                shouldTerminate = true;
                return null;
        }
        if(outputFrame != null){
            if(outputFrame.getName().equals("ERROR")){
                shouldTerminate = true;
            }
            return outputFrame;
        }
        HashMap<String, String> headers = new HashMap<>();
        headers.put("message", "Unknown command");
        return new StompFrame("ERROR", headers, "Invalid frame name, please use a valid frame name");

    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }



}
