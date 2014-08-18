/**
 * <p>Java client program that sends a message of given size to an echo server,
 * repeating a given number of times to measure an average TCP set-up time,
 * average message transmission time, and average socket tear-down time.</p>
 *
 * <p>Spring 2013 for CSCI 363 lab work.</p>
 * <p>Xiannong Meng<br>
 * xmeng@bucknell.edu<br></p>
 */
import java.io.*;
import java.net.*;
import java.util.Date;

class EchoClient   {

    public static void main(String[] args) throws IOException    {

	if (args.length != 4) {
	    System.err.println("usage : java EchoClient [msg_cnt] [msg_len] [server] [port]");
	    System.exit(1);
	}

	int count = Integer.parseInt(args[0]);  // repeating count
	int len   = Integer.parseInt(args[1]);  // desired message size
	String host = args[2];                  // server name
	int port  = Integer.parseInt(args[3]);  // server port

	long setupTime = 0;
	long msgTime = 0;
	long teardownTime = 0;

	long setupStart, setupEnd,
	    msgStart, msgEnd,
	    teardownStart, teardownEnd;

	long grandStart, grandEnd;

	long totalSetup = 0,
	    totalMsg = 0, 
	    totalTeardown = 0;

	String str = "";
	int size;

	// Construct the message
	for (int i = 0; i < len; i ++)
	    str += 'x';
	size = str.length();
	byte[] data = new byte[size];
	System.out.println("size of date in bytes : " + size);
	
	grandStart = new Date().getTime();

	for (int i = 0; i < count; i ++) { // repeat 'count' times
	    setupStart = new Date().getTime();
	    Socket t = new Socket(host, port);
	    setupEnd = new Date().getTime();

	    DataInputStream is = new DataInputStream(t.getInputStream());
	    DataOutputStream out = new DataOutputStream(t.getOutputStream());

	    msgStart = new Date().getTime();
	    // send out the size (number of bytes)
	    out.writeInt(size);
	    // send out the actual message
	    out.writeBytes(str);
	    // read the echoed message
	    is.readFully(data);
	    msgEnd = new Date().getTime();

	    teardownStart = new Date().getTime();
	    is.close();
	    out.close();
	    t.close();
	    teardownEnd = new Date().getTime();

	    totalSetup += setupEnd - setupStart;
	    totalMsg += msgEnd - msgStart;
	    totalTeardown += teardownEnd - teardownStart;
	    //	    System.out.println( "round " + i + "start/end " + setupStart + "/" + setupEnd );
	}  // for loop

	grandEnd = new Date().getTime();

	double avgSetup, avgMsg, avgTeardown;

	avgSetup = (double)totalSetup/count;
	avgMsg   = (double)totalMsg/count;
	avgTeardown = (double)totalTeardown/count;
	// print the average in mili-seconds
	System.out.printf("%6.4f %6.4f %6.4f %6.4f\n", avgSetup, avgMsg, avgTeardown, ((double)grandEnd-grandStart)/count);
    }  // main
}				       
