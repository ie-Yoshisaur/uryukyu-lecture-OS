import java.util.Enumeration;
import java.net.NetworkInterface;
import java.net.InetAddress;

public class Program7_2{
    public static void main(String[] args) {
        try {
            Enumeration interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface intf = (NetworkInterface)interfaces.nextElement();
                System.out.println("The name of this network interface is " + intf.getName() + ".");
                System.out.println("  All the IP addresses of this network interface are: ");
                Enumeration addresses = intf.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress address = (InetAddress)addresses.nextElement();
                    System.out.println("    " + address.getHostAddress());
                }
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}
