import socket

HOST = "192.168.229.206"  
PORT = 5000      

FILENAME = "received.txt"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()

    with conn:
        print(f"Conexión establecida desde {addr}")
        
        with open(FILENAME, 'wb') as f:
            print("Recibiendo archivo...")
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                f.write(data)
                
        print(f"Archivo '{FILENAME}' recibido con éxito.")