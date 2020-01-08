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

    private void init(String frame) {
        String[] lines=frame.split("\n");
        if(frame.length()>0)
            command=lines[0];
        for(int i=1;i<lines.length;i++){
            String currentLine=lines[i];
            if(currentLine.contains(":")){ //this line will be a header
                String[] header=currentLine.split(":");
                headers.put(header[0],header[1]);
            }
            else { //this line is part of the body
                body+=currentLine;
            }
        }
        //put the null sign in body
        if(body.length()>0){
            body+="\n";
        }
        body+="^@";
    }

    public String toString(){
        String result=command+"\n";
        for (String key:headers.keySet()) {
            result+=key+":"+headers.get(key)+"\n";
        }
        result+="\n";
        result+=body; //the body contains the null sign
        return result;
    }

}
