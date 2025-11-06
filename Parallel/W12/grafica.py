import matplotlib.pyplot as plt
import numpy as np

# Número de procesos
n_procesos = [1, 2, 4, 8, 16, 32, 64]

# Tiempos de ejemplo (en segundos, menores a 1 seg)
# REEMPLAZA ESTOS VALORES CON TUS TIEMPOS REALES
tiempos = [0.233452, 0.199062, 0.108731, 0.05409252, 0.0339078, 0.0205543, 0.017595]

# Crear la gráfica
plt.figure(figsize=(10, 6))
plt.plot(n_procesos, tiempos, marker='o', linewidth=2, markersize=8, label='Tiempo de ejecución')

# Configurar escala logarítmica en el eje y
plt.yscale('log')

# Etiquetas y título
plt.xlabel('Número de Procesos', fontsize=12)
plt.ylabel('Tiempo (segundos) - Escala Logarítmica', fontsize=12)
plt.title('Tiempo de Ejecución vs Número de Procesos', fontsize=14, fontweight='bold')

# Grid
plt.grid(True, which='both', linestyle='--', alpha=0.6)

# Leyenda
plt.legend(fontsize=10)

# Ajustar el eje x para mostrar todos los valores de procesos
plt.xticks(n_procesos)

# Mostrar valores en los puntos
for i, (x, y) in enumerate(zip(n_procesos, tiempos)):
    plt.annotate(f'{y:.6f}s', 
                xy=(x, y), 
                xytext=(0, 10), 
                textcoords='offset points',
                ha='center',
                fontsize=9,
                bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.3))

plt.tight_layout()
plt.savefig('tiempos_procesos.png', dpi=300, bbox_inches='tight')
plt.show()

print("Gráfica generada exitosamente!")
print(f"Número de procesos: {n_procesos}")
print(f"Tiempos (ejemplo): {tiempos}")
