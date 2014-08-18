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
import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLSocket;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;
import java.net.InetAddress;
import java.util.Date;
import java.text.DateFormat;
import java.text.SimpleDateFormat;

public
class EchoServer {
    public
            static
    void
            main(String[] arstring) {
        try {
            SSLServerSocketFactory sslserversocketfactory =
                    (SSLServerSocketFactory) SSLServerSocketFactory.getDefault();
            SSLServerSocket sslserversocket =
//                    (SSLServerSocket) sslserversocketfactory.createServerSocket(29999,4,InetAddress.getByName("linuxremote1.bucknell.edu"));
                    (SSLServerSocket) sslserversocketfactory.createServerSocket(29999);
            SSLSocket sslsocket = (SSLSocket) sslserversocket.accept();

            InputStream inputstream = sslsocket.getInputStream();
            InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
            BufferedReader bufferedreader = new BufferedReader(inputstreamreader);

            OutputStream outputstream = sslsocket.getOutputStream();
            OutputStreamWriter outputstreamwriter = new OutputStreamWriter(outputstream);
            BufferedWriter bufferedwriter = new BufferedWriter(outputstreamwriter);

	    /**
            String string = "hello world!";
	    string = bufferedreader.readLine();
	    System.out.println(string);
	    System.out.flush();
	    bufferedwriter.write("echo: " + string);
	    bufferedwriter.flush();
	    System.out.println("sent echo");
	    **/
	    String string = null;
            Date date = new Date();
            DateFormat df = new SimpleDateFormat("MM/dd/yyyy");
            while ((string = bufferedreader.readLine()) != null) {
                System.out.println(string);
                System.out.flush();
                /* Check the command from the client */
                if(string.equals("simple")){
                     string = df.format(date);
                }else if(string.equals("calendar")){
                     string = date.toString();
                }else{
                     string = "Invalid Command!";
                }
		bufferedwriter.write(string + '\n');
		bufferedwriter.flush();
		System.out.printf("sent %s\n", string);
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
    }
}
      
