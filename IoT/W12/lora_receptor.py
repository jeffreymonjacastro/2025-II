import serial
import time

# --- Configuración Principal ---
# ¡Cambia esto por el puerto COM de tu RAK Receptor!
PORT = 'COM15'  
BAUD_RATE = 115200
OUTPUT_FILE = 'log1.txt'

# ¡IMPORTANTE! Cambia la X por tu número de grupo
FREQ_GRUPO = "923500000"
# ---------------------------------

def send_at_command(ser, cmd):
    """Envía un comando AT al RAK y muestra la respuesta."""
    print(f"Enviando al RAK: {cmd}")
    ser.write((cmd + '\r\n').encode('utf-8'))
    
    # Espera un poco para que el RAK procese y responda
    time.sleep(2) 
    
    response = ""
    while ser.in_waiting > 0:
        response += ser.read(ser.in_waiting).decode('utf-8')
    
    if response:
        print(f"Respuesta del RAK: {response.strip()}")
    return response

try:
    # Abre el puerto serial
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    print(f"Conectado al RAK receptor en {PORT}...")

    # 1. Poner en modo LoRa P2P
    send_at_command(ser, "AT+NWM=0")
    
    # 2. Configurar parámetros P2P (Frecuencia, SF, BW, etc.)
    p2p_config_cmd = f"AT+P2P={FREQ_GRUPO}:7:125:0:10:14"
    send_at_command(ser, p2p_config_cmd)
    
    # 3. Poner en modo "Recepción Continua"
    send_at_command(ser, "AT+PRECV=65534")
    
    print("--- Configuración de Receptor P2P completada ---")
    print(f"Escuchando mensajes... Guardando en {OUTPUT_FILE} (Presiona Ctrl+C para parar)")
    
    # Abre el archivo donde guardarás los datos
    with open(OUTPUT_FILE, 'w') as f:
        while True:
            # Lee una línea del RAK
            linea = ser.readline().decode('utf-8').strip()
            
            # Si la línea no está vacía (ej. un mensaje +EVT), imprímela y guárdala
            if linea:
                print(linea)
                f.write(linea + '\n')
                # Forzar que se guarde en el disco inmediatamente (útil para logs en tiempo real)
                f.flush() 

except serial.SerialException as e:
    print(f"Error: No se pudo abrir el puerto {PORT}.")
    print("¿Está el puerto correcto? ¿Ya está siendo usado por otro programa (IDE de Arduino)?")
except KeyboardInterrupt:
    print("\nDeteniendo la captura de datos.")
finally:
    # Cierra el puerto serial al terminar
    if 'ser' in locals() and ser.is_open:
        # Opcional: enviar AT+PRECV=0 para detener el modo de recepción
        send_at_command(ser, "AT+PRECV=0")
        ser.close()
        print(f"Puerto serial {PORT} cerrado.")