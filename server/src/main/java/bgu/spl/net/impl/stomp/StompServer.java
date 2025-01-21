package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.echo.EchoProtocol;
import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if (args.length < 2) {
            return;
        }

        if (args[1] == "tpc") {
            Server.threadPerClient(
                    Integer.parseInt(args[0]), // port
                    () -> new EchoProtocol(), // protocol factory
                    LineMessageEncoderDecoder::new // message encoder decoder factory
            ).serve();
        }

        else if (args[1] == "reactor") {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    7777, // port
                    () -> new EchoProtocol<>(), // protocol factory
                    LineMessageEncoderDecoder::new // message encoder decoder factory
            ).serve();
        }
        return;
    }
}
