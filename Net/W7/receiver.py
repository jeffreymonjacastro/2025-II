import socket
import struct
import zlib
import random

class rdt_receiver:
    def __init__(self, host='127.0.0.1', port=5005, buffer_size=1024):
        self.host = host
        self.port = port
        self.buffer_size = buffer_size
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((self.host, self.port))
    
    def verify_checksum(self, data, received_checksum):
        calculated_checksum = zlib.crc32(data)
        return calculated_checksum == received_checksum
    
    def send_ack(self, packet_id, addr):
        ack_message = f"ACK {packet_id}".encode('utf-8')
        self.sock.sendto(ack_message, addr)
        print(f"Receiver: Sending ACK {packet_id}")
    
    def send_nak(self, packet_id, addr):
        nak_message = f"NAK {packet_id}".encode('utf-8')
        self.sock.sendto(nak_message, addr)
        print(f"Receiver: Sending NAK {packet_id}")
    
    def receive_packet(self):
        data, addr = self.sock.recvfrom(self.buffer_size)
        
        header_size = struct.calcsize('!IIII')
        
        udp_header = data[:header_size]
        source_port, dest_port, data_length, checksum = struct.unpack('!IIII', udp_header)
        
        message_bytes = data[header_size:]
        message = message_bytes.decode('utf-8')
        
        packet_id = int(message.split()[1])
        
        is_valid = self.verify_checksum(message_bytes, checksum)
        
        if is_valid:
            print(f"Receiver: Successfully received Packet {packet_id}. Sending ACK.")
            self.send_ack(packet_id, addr)
        else:
            print(f"Receiver: Packet {packet_id} corrupted. Sending NAK.")
            self.send_nak(packet_id, addr)
        
        return packet_id, is_valid
    
    def start_listening(self, num_packets=10):
        while True:
            self.receive_packet()
    
    def close(self):
        self.sock.close()


def main():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 5005
    
    receiver = rdt_receiver(host=UDP_IP, port=UDP_PORT)
    
    receiver.start_listening(num_packets=10)
    
    receiver.close()


if __name__ == "__main__":
    main()
