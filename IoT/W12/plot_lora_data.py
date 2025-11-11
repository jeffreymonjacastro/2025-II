import matplotlib.pyplot as plt
import numpy as np

# Leer el archivo log1.txt
def parse_log_file(filename):
    """
    Parse el archivo de log con formato +EVT:RXP2P:POT:SNR:PAYLOAD
    Retorna listas de potencia, SNR y tiempo
    """
    potencias = []
    snrs = []
    tiempos = []
    
    with open(filename, 'r') as file:
        for idx, line in enumerate(file):
            line = line.strip()
            if line.startswith('+EVT:RXP2P:'):
                # Dividir la línea por ':'
                parts = line.split(':')
                if len(parts) >= 5:
                    try:
                        pot = int(parts[2])  # Potencia
                        snr = int(parts[3])  # SNR
                        
                        potencias.append(pot)
                        snrs.append(snr)
                        tiempos.append(idx * 2)  # Usar el índice como tiempo
                    except ValueError:
                        # Ignorar líneas con formato incorrecto
                        continue
    
    return tiempos, potencias, snrs

# Parsear el archivo
tiempos, potencias, snrs = parse_log_file('log1.txt')

# Crear la figura con dos subgráficas
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 8))

# Gráfica 1: Potencia vs Tiempo
ax1.plot(tiempos, potencias, marker='o', linestyle='-', color='blue', linewidth=2, markersize=4)
ax1.set_xlabel('Tiempo (segundos)', fontsize=12)
ax1.set_ylabel('Potencia (dBm)', fontsize=12)
ax1.set_title('Potencia de Señal LoRa P2P vs Tiempo', fontsize=14, fontweight='bold')
ax1.grid(True, alpha=0.3)
ax1.axhline(y=np.mean(potencias), color='r', linestyle='--', label=f'Promedio: {np.mean(potencias):.2f} dBm')
ax1.legend()

# Gráfica 2: SNR vs Tiempo
ax2.plot(tiempos, snrs, marker='s', linestyle='-', color='green', linewidth=2, markersize=4)
ax2.set_xlabel('Tiempo (segundos)', fontsize=12)
ax2.set_ylabel('SNR (dBm)', fontsize=12)
ax2.set_title('Signal-to-Noise Ratio (SNR) vs Tiempo', fontsize=14, fontweight='bold')
ax2.grid(True, alpha=0.3)
ax2.axhline(y=np.mean(snrs), color='r', linestyle='--', label=f'Promedio: {np.mean(snrs):.2f} dBm')
ax2.legend()

# Ajustar el espaciado entre subgráficas
plt.tight_layout()

# Guardar la figura
plt.savefig('lora_analysis.png', dpi=300, bbox_inches='tight')
print("Gráfica guardada como 'lora_analysis.png'")

# Mostrar estadísticas
print("\n=== Estadísticas de la transmisión LoRa P2P ===")
print(f"Número de muestras: {len(tiempos)}")
print(f"\nPotencia:")
print(f"  - Promedio: {np.mean(potencias):.2f} dBm")
print(f"  - Mínimo: {np.min(potencias)} dBm")
print(f"  - Máximo: {np.max(potencias)} dBm")
print(f"  - Desviación estándar: {np.std(potencias):.2f} dBm")
print(f"\nSNR:")
print(f"  - Promedio: {np.mean(snrs):.2f} dBm")
print(f"  - Mínimo: {np.min(snrs)} dBm")
print(f"  - Máximo: {np.max(snrs)} dBm")
print(f"  - Desviación estándar: {np.std(snrs):.2f} dBm")

# Mostrar las gráficas
plt.show()
