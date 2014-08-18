/**
 * The following example is copied from
 * http://stilius.net/java/java_ssl.php
 * on 2013-04-16
 * by Xiannong Meng for CSCI 363 spring 2013 at Bucknell
 *
 * Author's original copy-right note:
 *
 * Copyright
 * This document is copyrighted to Tomas Vilda. You can use it in all ways, but don't change this section and allways include it.
 *
 */
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import java.io.*;
import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Random;


public
class EchoClient {
    public static void main(String[] arstring) {
        
        /* Test the Console */
        Console console = System.console();
        Random generator = new Random( 1234 ); // the parameter is a seed
        int r, number;

        do {
            r = generator.nextInt(Integer.MAX_VALUE) % 1000000;
            System.out.println("Random number generated : " + r);

            console.printf("Please type the number seen above: ");

            // the readPassword() method turns off the input echo
            char[] charsTyped = console.readPassword();
            String numString = new String(charsTyped);
            number = Integer.parseInt(numString);

            if (number == r)
                System.out.println("Great!");
            else
                System.out.println("Number typed is incorrect, try again.");
        } while (number != r);
                        

	String host;
	if (arstring.length == 2)
	    host = arstring[1];
	else
	    host = "localhost";

        try {
            SSLSocketFactory sslsocketfactory = (SSLSocketFactory) SSLSocketFactory.getDefault();
            SSLSocket sslsocket = (SSLSocket) sslsocketfactory.createSocket(host, 29999);

            InputStream inputstream = System.in;
            InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
            BufferedReader bufferedreader = new BufferedReader(inputstreamreader);

            OutputStream outputstream = sslsocket.getOutputStream();
            OutputStreamWriter outputstreamwriter = new OutputStreamWriter(outputstream);
            BufferedWriter bufferedwriter = new BufferedWriter(outputstreamwriter);
            InputStream inputstreamSock = sslsocket.getInputStream();
            InputStreamReader inputstreamreaderSock = new InputStreamReader(inputstreamSock);
            BufferedReader bufferedreaderSock = new BufferedReader(inputstreamreaderSock);

            String string = null;
	    String echo = null;
	    System.out.print("Type a command (simple OR calendar) : ");
            while ((string = bufferedreader.readLine()) != null) {
		bufferedwriter.write(string + '\n');
                bufferedwriter.flush();
		System.out.println("sent command: "+string);
		echo = bufferedreaderSock.readLine();
		System.out.println(echo);
		System.out.flush();
		System.out.print("Type a command (simple OR calendar) : ");
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
    }
}
     
