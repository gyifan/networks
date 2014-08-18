/**
 * <p>Java server program echos client's message.</p>
 *
 * <p>Spring 2013 for CSCI 363 lab work.</p>
 * <p>Xiannong Meng<br>
 * xmeng@bucknell.edu<br></p>
 */
import java.io.*;
import java.net.*;

class EchoServer     {

    public static void main(String[] args)    {

	if (args.length != 1) {
	    System.err.println("usage java EchoServer.java [port]");
	    System.exit(1);
	}

	int port = Integer.parseInt(args[0]);

	try   {
	    ServerSocket s = new ServerSocket(port);
	    while (true)  {
		Socket incoming = s.accept();
		DataInputStream in
		    = new DataInputStream(incoming.getInputStream());
		DataOutputStream out
		    = new DataOutputStream(incoming.getOutputStream());

		int size = in.readInt();
		byte[] data = new byte[size];
		in.readFully(data);
		out.writeBytes(new String(data));
		incoming.close();
	    } // end of while
	}   catch (Exception e) {
	    System.out.println(e);
	}  // end of try-catch
    }  // end of main()
}
