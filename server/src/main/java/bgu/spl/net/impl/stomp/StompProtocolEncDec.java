package bgu.spl.net.impl.stomp;

import java.util.Arrays;
import java.nio.charset.StandardCharsets;
import bgu.spl.net.api.MessageEncoderDecoder;

public class StompProtocolEncDec  implements MessageEncoderDecoder<StompFrame> {

    private StringBuilder frameBuffer = new StringBuilder();

    public StompFrame decodeNextByte(byte nextByte) {
        if (nextByte == 0) {
            return parseFrame();
        }

        frameBuffer.append((char)nextByte);
        return null;
    }

    public byte[] encode(StompFrame frame) {
        StringBuilder frameBytes = new StringBuilder();
        frameBytes.append(frame.toString());
        frameBytes.append("\u0000");
        System.out.println("ENCODED FRAME: ");
        System.out.println(frameBytes.toString());
        return frameBytes.toString().getBytes(StandardCharsets.UTF_8);
    }

    private StompFrame parseFrame() {
        String frameString = frameBuffer.toString();
        frameString.chars();
        frameBuffer.setLength(0);
        return new StompFrame(frameString);
    }
}
