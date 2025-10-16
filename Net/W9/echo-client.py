import socket

HOST = "127.0.0.1"
PORT = 10514

FILENAME = "rfc793.txt" 

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    
    with open(FILENAME, 'rb') as f:
        file_data = f.read()

    print(f"Enviando el archivo '{FILENAME}' ({len(file_data)} bytes)...")
    s.sendall(file_data)
            
    print("Archivo enviado completamente.")