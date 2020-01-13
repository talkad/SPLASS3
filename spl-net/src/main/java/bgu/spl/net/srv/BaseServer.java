package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

public abstract class BaseServer<T> implements Server<T> {

    private final int port;
    private final Supplier<StompMessagingProtocol> protocolFactory;
    private final Supplier<MessageEncoderDecoder<Frame>> encdecFactory;
    private ServerSocket sock;
    private Connections connections;
    private int connectionID;

    public BaseServer(
            int port,
            Supplier<StompMessagingProtocol> protocolFactory,
            Supplier<MessageEncoderDecoder<Frame>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
		this.sock = null;
        connectionID=0;
        connections=new ConnectionsImpl();
    }

    @Override
    public void serve() {

        try (ServerSocket serverSock = new ServerSocket(port)) {
			System.out.println("Server started");

            this.sock = serverSock; //just to be able to close

            while (!Thread.currentThread().isInterrupted()) {

                Socket clientSock = serverSock.accept();

                BlockingConnectionHandler handler = new BlockingConnectionHandler(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get(),
                        connectionID,
                        connections);
                connections.addConnection(connectionID,handler);
                execute(handler);
            }
        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
		if (sock != null)
			sock.close();
    }

    protected abstract void execute(BlockingConnectionHandler  handler);

}
