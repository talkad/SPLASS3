package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.*;

import java.util.function.Supplier;

public class StompServer {

    public static void main(String[] args) {


        int port=0;
        Supplier<MessageEncoderDecoder<Frame>> encoderDecoderFactory= StompEncoderDecoder::new;
        Supplier<StompMessagingProtocol> protocolFactory= StompMessagingProtocolImpl::new;
        //StompThreadPerClient( port, protocolFactory, encoderDecoderFactory).serve();
        StompReactor(8,port,protocolFactory,encoderDecoderFactory).serve();


    }
    public static  Server<Frame>  StompThreadPerClient(
            int port,
            Supplier<StompMessagingProtocol> protocolFactory,
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
        return new Reactor<Frame>(nthreads, port, protocolFactory, encoderDecoderFactory);
    }


}

