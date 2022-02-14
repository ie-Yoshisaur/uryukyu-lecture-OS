import java.net.InetAddress;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Source2 {
    public static void main(String[] args) {
        try {
            int port = 8888;
            String dataToSend = "Hello, World!";
            byte[] data = dataToSend.getBytes("UTF-8");
            InetAddress address = InetAddress.getByName("255.255.255.255");
            DatagramPacket packet = new DatagramPacket(data, data.length, address, port);
            DatagramSocket socket = new DatagramSocket();
            socket.send(packet);
            socket.close();
	} catch (Exception e) {
            System.err.println(e);
	}
    }
}
