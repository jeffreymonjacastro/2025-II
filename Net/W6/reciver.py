import socket

# Dirección IP de la máquina receptora (usar ipconfig/ifconfig para saber tu IP LAN)
UDP_IP = "192.168.1.10"   # ejemplo, cambia por la IP local de tu laptop receptora
UDP_PORT = 5005

# Crear socket UDP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on {UDP_IP}:{UDP_PORT}")

while True:
    data, addr = sock.recvfrom(1024)  # buffer de 1024 bytes
    print(f"Received message: {data} from {addr}")
