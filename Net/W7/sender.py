import socket
import zlib
import struct
import time
from random import randint

class rdt_sender:
    def __init__(self, host='127.0.0.1', port=5005):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.settimeout(2.0)
    
    def checksum_calculator(self, data):
        checksum = zlib.crc32(data)
        return checksum
    
    def is_corrupted(self):
        return randint(0, 9) < 8
    
    def send_packet(self, packet_id):
        message = f"Packet {packet_id}"
        message_bytes = message.encode('utf-8')
        data_length = len(message_bytes)
        checksum = self.checksum_calculator(message_bytes)
        
        if self.is_corrupted():
            message_bytes = "Corrupted 1".encode('utf-8')
        
        udp_header = struct.pack('!IIII', self.port, self.port, data_length, checksum)
        final_message = udp_header + message_bytes
        
        print(f"Sender: Sending Packet {packet_id}")
        self.sock.sendto(final_message, (self.host, self.port))
        
        data, addr = self.sock.recvfrom(1024)
        response = data.decode('utf-8')
        
        if response == f"ACK {packet_id}":
            print(f"Sender: Received ACK {packet_id}")
            print(f"Sender: ACK received for Packet {packet_id}")
            return True
        elif response == f"NAK {packet_id}":
            print(f"Sender: Received NAK {packet_id}")
            print(f"Sender: Resending Packet {packet_id}")
            return self.send_packet(packet_id)
    
    def send_multiple_packets(self, num_packets):
        for packet_id in range(num_packets):
            self.send_packet(packet_id)
            time.sleep(0.3)
    
    def close(self):
        self.sock.close()


def main():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5005
    
    sender = rdt_sender(host=UDP_IP, port=UDP_PORT)
    
    NUM_PACKETS = 10
    sender.send_multiple_packets(NUM_PACKETS)
    
    sender.close()


if __name__ == "__main__":
    main()