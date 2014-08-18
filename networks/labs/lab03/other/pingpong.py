############################################################################
# Client side: use sockets to send data to the server, and print server's 
# reply to each message line; 'localhost' means that the server is running
# on the same machine as the client, which lets us test client and server 
# on one machine;  to test over the Internet, run a server on a remote
# machine, and set serverHost or argv[1] to machine's domain name or IP addr;
# Python sockets are a portable BSD socket interface, with object methods 
# for the standard socket calls available in the sytstem's C library;
############################################################################

# revised by X. Meng 01-19-2013 for CSCI 363, converted it to Python 3.x and
# removed extra elements, using only the minimum setup.
#
# X Meng 01-22-2013 for CSCI 363
# We also added the timing components to measure the time needed
# to set up and tear down the sockets, as well as the transmission time
# for a fix number of bytes between the server and the client.
     
import sys
import string
from socket import *              # portable socket interface plus constants
import time

if __name__ == "__main__":

    if len( sys.argv ) != 5:
        print("usage python pingpong.py [num_msgs] [msg_length] [hostname] [port]");
        sys.exit(1);

    # take up the values of the arguments
    count = (int)(sys.argv[1]);
    cntst = sys.argv[1];
    leng  = (int)(sys.argv[2]);
    host  = sys.argv[3];
    port  = (int)(sys.argv[4]);

    # make the message according to the length
    message = ''
    for i in range( leng ):
        message += 'x'
    # encode the message so socket can send it
    datasent = str.encode(message)
    
    # compute the size of the data to be sent
    bytes_to_send = sys.getsizeof(datasent)
    size_in_bin   = str.encode(str(bytes_to_send)+'#')  # separate size by '#'
     
    total_setup = 0     # socket set-up time
    total_msg   = 0     # message passing time
    total_trdn  = 0     # socket tear-down time

    grand_start = time.time()
    for i in range( count ):
        setup_start = time.time()
        sockobj = socket(AF_INET, SOCK_STREAM) # make a TCP/IP socket object
        sockobj.connect((host, port))          # connect to server 
        setup_end = time.time()
    
        msg_start = setup_end
        sockobj.send(size_in_bin)              # send over the size first
        sockobj.send(datasent)                 # send the actual data
        bytes_to_receive = bytes_to_send
        recv_str = ''
        received_count = 0
        #print('C: send all at client, bytes to receive ', bytes_to_receive)
        while bytes_to_receive > 0:
            data = sockobj.recv(bytes_to_send) # receive echoed data
            if not data: break                 # end the loop
            recv_str += bytes.decode(data)
            received_count += sys.getsizeof(data)
            bytes_to_receive -= received_count
        # print('C: total received at client ', received_count)

        msg_end = time.time()
        #    print('sent->', message)
        #    print('recv->', recv_str)
        trdn_start = msg_end;
        sockobj.close()            # close socket to send eof to server
        trdn_end = time.time()
        
        total_setup += (setup_end - setup_start)
        total_msg   += (msg_end - msg_start)
        total_trdn  += (trdn_end - trdn_start)

    grand_end = time.time()
    # print(total_setup, total_msg, total_trdn)
    # print time in mili-seconds
    print("%6.4f" % (float(total_setup*1000.0)/float(count)), end = '  ')
    print("%6.4f" % (float(total_msg*1000.0)/float(count)), end = '  ')
    print("%6.4f" % (float(total_trdn*1000.0)/float(count)), end = ' ')
    print("%6.4f" % (float((grand_end - grand_start)*1000.0/float(count))))
