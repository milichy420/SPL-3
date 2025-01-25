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

    @Override
    public StompFrame process(StompFrame msg) {
        System.out.println("IM PRINTING THE MESSAGE");
        System.out.println(msg.toString());
        StompFrame outputFrame = null;
        System.out.println("the message name is: " + msg.getName());
        switch (msg.getName()) {
            case "CONNECT":
                outputFrame = FrameHandler.handleConnect(msg, connections, connectionId);
                System.out.println("output frame is: " + outputFrame.toString());
            case "SEND":
                
                break;
            case "SUBSCRIBE":
                
                break;
            case "UNSUBSCRIBE":
                
                break;
            case "DISCONNECT":    
                shouldTerminate = true;
                break;
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
