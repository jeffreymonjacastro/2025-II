import socket

HOST = "127.0.0.1"  
PORT = 10514      

FILENAME = "rfc_recibido.txt"

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