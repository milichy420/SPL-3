package bgu.spl.net.impl.stomp;

import java.util.HashMap;

public class StompFrame {
    private String name;
    private String body;
    private HashMap<String,String> headers;

    public StompFrame(String name, HashMap<String,String> headers, String body){
        this.name = name;
        this.headers = headers;
        this.body = body;
    }

    public StompFrame(String frameString){
        String[] lines = frameString.split("\n");
        this.name = lines[0];
        this.headers = new HashMap<>();
        int i = 1;
        while (lines.length > i && !lines[i].equals("")){
            String[] header = lines[i].split(":");
            headers.put(header[0],header[1]);
            i++;
        }

        this.body = "";
        while (i < lines.length){
            this.body += lines[i] + "\n";
            i++;}
        }


    public String getName() {
        return name;
    }

    public String getBody() {
        return body;
    }

    public HashMap<String, String> getHeaders() {
        return headers;
    }

    public String toString(){
        String result = name + "\n";
        for(String key : headers.keySet()){
            result += key + ":" + headers.get(key) + "\n";
        }
        result += "\n";
        result += body;
        return result;
    }
}

