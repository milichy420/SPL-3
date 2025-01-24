package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: StompServer <port> <tpc/reactor>");
            return;
        }

        if (args[1] == "tpc") {
            Server.threadPerClient(
                    Integer.parseInt(args[0]), // port
                    () -> new StompProtocol(), // protocol factory
                    LineMessageEncoderDecoder::new // message encoder decoder factory
            ).serve();
        }

        else if (args[1] == "reactor") {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    7777, // port
                    () -> new StompProtocol(), // protocol factory
                    LineMessageEncoderDecoder::new // message encoder decoder factory
            ).serve();
        }
        return;
    }
}
