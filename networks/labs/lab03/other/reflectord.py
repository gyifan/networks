############################################################################
# Server side: open a TCP/IP socket on a port, listen for a message from
# a client, and send an echo reply; this is a simple one-shot listen/reply
# conversation per client, but it goes into an infinite loop to listen for
# more clients as long as this server script runs; the client may run on 
# a remote machine, or on same computer if it uses 'localhost' for server
############################################################################

# revised by X. Meng 01-19-2013 for CSCI 363, converted it to Python 3.x and
# removed extra elements, using only the minimum setup.
# We also added the timing components to measure the time needed
# to set up and tear down the sockets, as well as the transmission time
# for a fix number of bytes between the server and the client.

import sys
import string
from socket import *                    # get socket constructor and constants

def extract_size( data ):
    '''extract the 'size' (number of bytes) information
    from the received data, and separate the data.'''
    data_len = len(data)
    i = 0
    size = ''
    while i < data_len:
        if chr(data[i]) >= '0' and chr(data[i]) <= '9': # a digit
            size += chr(data[i])
        else:
            break         # end the loop
        i = i + 1

    return int(size), data[i:]

if __name__ == "__main__":

    if len( sys.argv ) != 2:
        print("usage python reflectord.py [port]");
        sys.exit(1);

    buffsize = 4096
    host = ''                             # server machine, '' means local host
    port = (int)(sys.argv[1])

    sockobj = socket(AF_INET, SOCK_STREAM)       # make a TCP socket object
    sockobj.bind((host, port))               # bind it to server port number 
    sockobj.listen(5)                        # listen, allow 5 pending connects
     
    while True:                              # listen until process killed
        connection, address = sockobj.accept()  # wait for next client connect
#        print('S: Server connected by', address)   # connection is a new socket
        data = connection.recv(buffsize)        # read the size of data first
        size,data = extract_size(data);
#        print('S: server received size', size)
        received_bytes = sys.getsizeof(data)
        connection.send(data)                # echo back
        while received_bytes < size:
 #           print('S: server sent data')
            data = connection.recv(int(size))    # read next portion
            if not data: break
            received_bytes += sys.getsizeof(data)
#            print('S: total received bytes ', received_bytes)
            connection.send(data)                # echo back
        # print('S: grand total received bytes ', received_bytes)
        connection.close()                       # until eof when socket closed


        
    
    
