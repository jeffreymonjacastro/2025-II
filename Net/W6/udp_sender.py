import socket
import zlib
import struct

#checksum generation function
def checksum_calculator(data):
    checksum = zlib.crc32(data)
    return checksum

UDP_IP = "10.74.203.68"
UDP_PORT = 5005
MESSAGE = b"Holaaaaaa"

print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)
print("message: %s" % MESSAGE)

data_length = len(MESSAGE)
print("Data length: %s" % data_length)
#checksum = checksum_calculator(MESSAGE)
checksum = int("4", 16) #forced checksum value
print("Checksum: %s" % checksum)

#Header construction
udp_header = struct.pack("!IIII", UDP_PORT, UDP_PORT, data_length, checksum)
print ("Header: %s" % udp_header)
MESSAGE = udp_header + MESSAGE #concatenation

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE,(UDP_IP, UDP_PORT))