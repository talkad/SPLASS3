package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

public class BlockingConnectionHandler implements Runnable, ConnectionHandler<Frame> {

    private final StompMessagingProtocol protocol;
    private final MessageEncoderDecoder<Frame> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private int connectionId;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<Frame> reader, StompMessagingProtocol protocol,int connectionId) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.connectionId=connectionId;
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;

            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());

            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                Frame nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    protocol.process(nextMessage);
                }
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(Frame msg) {
        //IMPLEMENT IF NEEDED
    }
}
