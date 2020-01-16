package bgu.spl.net.srv;

import java.util.Hashtable;

public class Frame {
    private String command;
    private Hashtable<String,String> headers;
    private String body;

    public Frame(String frame){
        command="";
        body="";
        headers=new Hashtable<>();
        init(frame);
    }

    public Frame(String command,String body){
        this.command=command;
        this.body=body;
        headers=new Hashtable<>();
    }

    private void init(String frame) {
        String[] lines=frame.split("\n");
        if(frame.length()>0)
            command=lines[0];
        int i=1;
        String currentLine=lines[1];
        while(currentLine.contains(":")) {
            //this line will be a header
            String[] header = currentLine.split(":");
            headers.putIfAbsent(header[0], header[1]);
            i++;
            currentLine=lines[i];
        }
        //if we exited the while we are now on "" that separates headers and body
        //this line is part of the body
        currentLine=lines[i+1];
        if(currentLine.equals("^@"))
            body ="";
        else
            body=currentLine;
    }

    public void addHeader(String key,String value){
        headers.put(key,value);
    }

    public String toString(){
        String result=command+"\n";
        for (String key:headers.keySet()) {
            result+=key+":"+headers.get(key)+"\n";
        }
        result+="\n";
        result+=body;
        if(body.equals(""))
            result+="^@";
        else
            result+="\n^@";
        return result;
    }

    public String getCommand() {
        return command;
    }

    public Hashtable<String, String> getHeaders() {
        return headers;
    }

    public String getBody() {
        return body;
    }
}
