package bgu.spl.net.impl.stomp;

import java.util.HashMap;

import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Connections;

public class FrameHandler {

    public static StompFrame handleConnect(StompFrame msg, Connections<StompFrame> connections, int connectionId) {
        HashMap<String, String> inputHeaders = msg.getHeaders();
        HashMap<String, String> outputHeaders = new HashMap<>();
        String outputBody = "";
        String outputName = "";
        StompFrame outputFrame;
        if (!inputHeaders.containsKey("accept-version") || !inputHeaders.containsKey("host")) {
            outputName = "ERROR";
            outputHeaders.put("message", "missing headers error");
            outputBody = "missing headers, must include accept-version and host";
        } else if (!inputHeaders.get("accept-version").equals("1.2")) {
            outputName = "ERROR";
            outputHeaders.put("message", "version error");
            outputBody = "version not supported, please use version 1.2";
        } else if (!inputHeaders.get("host").equals("stomp.cs.bgu.ac.il")) {
            outputName = "ERROR";
            outputHeaders.put("message", "host error");
            outputBody = "host not supported, please use stomp.cs.bgu.ac.il";
        } else if (!inputHeaders.containsKey("login") || !inputHeaders.containsKey("passcode")) {
            outputName = "ERROR";
            outputHeaders.put("message", "missing headers error");
            outputBody = "the login and passcode headers must be present";
        } else {
            String loginReply = connections.login(inputHeaders.get("login"), inputHeaders.get("passcode"),
                    connectionId);
                System.out.println("loginReply: " + loginReply);
            if (loginReply == "already logged in") {
                HashMap<String, String> headers = new HashMap<>();
                headers.put("message", String.valueOf("Login failed"));
                return new StompFrame("ERROR", headers, "User already logged in");
            }
            else if(loginReply.equals("connected")){
                HashMap<String, String> headers = new HashMap<>();
                headers.put("version", "1.2");
                return new StompFrame("CONNECTED", headers, "");
            }
            else{
                HashMap<String, String> headers = new HashMap<>();
                headers.put("message", String.valueOf("Login failed"));
                return new StompFrame("ERROR", headers, "Wrong password for user: " + loginReply); 
            }
        }
        return new StompFrame(outputName, outputHeaders, outputBody);
    }

    public static StompFrame handleSend(StompFrame frame) {
        System.out.println(frame.toString());
        return null;
    }

    public static StompFrame handleSubscribe(StompFrame frame) {
        System.out.println(frame.toString());
        return null;
    }

    public static StompFrame handleUnsubscribe(StompFrame frame) {
        System.out.println(frame.toString());
        return null;
    }

    public static void handleDisconnect(StompFrame msg, Connections<StompFrame> connections, int connectionId) {
        connections.disconnect(connectionId);
    }

    public static void handleReceipt(StompFrame frame, Connections<StompFrame> connections, int connectionId) {
        String receiptId = frame.getHeaders().get("receipt");
        HashMap<String, String> headers = new HashMap<>();
        headers.put("receipt-id", receiptId); 
        StompFrame receiptFrame = new StompFrame("RECEIPT", headers, "");
        connections.send(connectionId, receiptFrame);
    }

}
