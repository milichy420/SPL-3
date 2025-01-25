package bgu.spl.net.impl.stomp;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: StompServer <port> <tpc/reactor>");
            return;
        }

        if (args[1].equals("tpc")) {
            Server.threadPerClient(
                    Integer.parseInt(args[0]), // port
                    () -> new StompProtocol(), // protocol factory
                    StompProtocolEncDec::new // message encoder decoder factory
            ).serve();
        }

        else if (args[1].equals("reactor")) {
            Server.reactor(
                    Runtime.getRuntime().availableProcessors(),
                    Integer.parseInt(args[0]), // port
                    () -> new StompProtocol(), // protocol factory
                    StompProtocolEncDec::new // message encoder decoder factory
            ).serve();
        }
        return;
    }
}
