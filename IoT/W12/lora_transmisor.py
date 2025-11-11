import serial
import time

PORT = 'COM10'
BAUD_RATE = 115200

FREQ_GRUPO = "923500000"


def string_to_hex(s):
    """Convierte un string (ej: '123') a su payload en Hex (ej: '313233')"""

    return ''.join(f"{ord(c):02X}" for c in s)

def send_at_command(ser, cmd):
    """Envía un comando AT al RAK y muestra la respuesta."""

    print(f"Enviando al RAK: {cmd}")
    ser.write((cmd + '\r\n').encode('utf-8'))
    
    time.sleep(2) 
    
    response = ""
    while ser.in_waiting > 0:
        response += ser.read(ser.in_waiting).decode('utf-8')
    
    if response:
        print(f"Respuesta del RAK: {response.strip()}")
    return response

try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    print(f"Conectado al RAK transmisor en {PORT}...")

    send_at_command(ser, "AT+NWM=0")
    
    p2p_config_cmd = f"AT+P2P={FREQ_GRUPO}:7:125:0:10:14" 
    send_at_command(ser, p2p_config_cmd)
    
    send_at_command(ser, "AT+PRECV=0") # [cite: 176]
    
    print("--- Configuración de Transmisor P2P completada ---")
    print("--- Iniciando bucle de transmisión (Ctrl+C para parar) ---")

    counter = 0
    while True:
        mensaje_str = str(counter)
        
        mensaje_hex = string_to_hex(mensaje_str)
        
        comando_send = f"AT+PSEND={mensaje_hex}"
        send_at_command(ser, comando_send)
        
        counter += 1
        
        time.sleep(2)

except serial.SerialException as e:
    print(f"Error: No se pudo abrir el puerto {PORT}.")
    print("¿Está el puerto correcto? ¿Está siendo usado por el IDE de Arduino?")
except KeyboardInterrupt:
    print("\nDeteniendo la transmisión.")
finally:
    # Cierra el puerto serial al terminar
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print(f"Puerto serial {PORT} cerrado.")
