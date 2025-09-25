import socket

# Dirección IP y puerto de la computadora receptora
UDP_IP = "192.168.1.10"   # cambiar por la IP de la laptop que recibe
UDP_PORT = 5005
MESSAGE = b"Hello from sender!"

print(f"Sending message to {UDP_IP}:{UDP_PORT}")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
