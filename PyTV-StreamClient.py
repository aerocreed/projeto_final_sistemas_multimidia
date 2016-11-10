import socket
import sys

#Variaveis
BUFSIZE = 167535

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('localhost', 20000)
print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)
try:

    # Send data
    message = '1,1'
    print >>sys.stderr, 'sending "%s"' % message
    sock.send(message)

    i=0

    myfile = open("videosout/testfile"+str(i).zfill(3)+".webm", 'w')
    while True:
        data = sock.recv(BUFSIZE)
        if not data: break
        if ("AAAAFFFFFFGGGGGGQQQQQQQQQ" in data):
            myfile.close()
            i = i + 1
            myfile = open("videosout/testfile"+str(i).zfill(3)+".webm", 'w')
            print 'Entrou Aqui !'
        else:
            myfile.write(data)
        print 'writing file 2....'

    myfile.close()

#    while amount_received < amount_expected:
#        data = sock.recv(16)
#        amount_received += len(data)
#        print >>sys.stderr, 'received "%s"' % data

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()