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

        frameBuffer.append(nextByte);
        return null;
    }

    public byte[] encode(StompFrame frame) {
        StringBuilder frameBytes = new StringBuilder();
        frameBytes.append(frame.getName());
        for(String key : frame.getHeaders().keySet()){
            frameBytes.append(key + ":" + frame.getHeaders().get(key) + "\n");
        }
        frameBytes.append("\n");
        frameBytes.append(frame.getBody());
        frameBytes.append("\u0000");
        return frameBytes.toString().getBytes(StandardCharsets.UTF_8);
    }

    private StompFrame parseFrame() {
        String frameString = frameBuffer.toString();
        frameBuffer.setLength(0);
        return new StompFrame(frameString);
    }
}
