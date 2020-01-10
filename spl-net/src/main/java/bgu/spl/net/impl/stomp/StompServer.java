package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.*;

import java.util.function.Supplier;

import static bgu.spl.net.srv.Server.reactor;
import static bgu.spl.net.srv.Server.threadPerClient;

public class StompServer {

    public static void main(String[] args) {

        boolean TPS=true;
        int port=0;
        Supplier<MessageEncoderDecoder<Frame>> encoderDecoderFactory= StompEncoderDecoder::new;
        Supplier<StompMessagingProtocol> protocolFactory= StompMessagingProtocol::new;

        if(args[0]=="TPS") {//tread per client
            StompThreadPerClient( port, protocolFactory, encoderDecoderFactory).serve();
        }
        else{
            StompReactor(8,port,protocolFactory,encoderDecoderFactory).serve();
        }

    }
    public static  Server<Frame>  StompThreadPerClient(
            int port,
            Supplier<StompMessagingProtocol > protocolFactory,
            Supplier<MessageEncoderDecoder<Frame> > encoderDecoderFactory) {

        return new BaseServer<Frame>(port, protocolFactory, encoderDecoderFactory) {
            @Override
            protected void execute(BlockingConnectionHandler  handler) {
                new Thread(handler).start();
            }
        };
    }
    public static  Server<Frame> StompReactor(
            int nthreads,
            int port,
            Supplier<StompMessagingProtocol> protocolFactory,
            Supplier<MessageEncoderDecoder<Frame>> encoderDecoderFactory) {
        return new Reactor<>(nthreads, port, protocolFactory, encoderDecoderFactory);
    }


}

