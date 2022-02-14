import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.DatagramPacket;
import java.util.Arrays;

public class ReceiveMulticast{
    public static void main(String[] args) {
        try {
            int port = 8888;
            MulticastSocket socket = new MulticastSocket(port);
            InetAddress multicastAddress = InetAddress.getByName("239.1.14.126");
            socket.joinGroup(multicastAddress);
            byte[] data = new byte[1024];
            DatagramPacket packet = new DatagramPacket(data, data.length);
            socket.receive(packet);
            System.out.println(new String(Arrays.copyOf(packet.getData(), packet.getLength()), "UTF-8"));
            socket.close();
	} catch (Exception e) {
            System.err.println(e);
	}
    }
}
