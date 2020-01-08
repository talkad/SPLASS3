package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;

public class StompMessageIncoderDecoder<T> implements MessageEncoderDecoder<T> {

    @Override
    public T decodeNextByte(byte nextByte) {
        return null;
    }

    @Override
    public byte[] encode(T message) {
        return new byte[0];
    }
}
