#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

static inline double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

int main() {
    const int n = 200000;  // número de muestras
    const int d = 64;      // número de características

    // Inicialización
    vector<double> w(d, 0.0);     // pesos
    vector<double> grad(d, 0.0);  // gradiente acumulado
    vector<double> X(n * d);
    vector<double> y(n);

    // Datos sintéticos
    for (int i = 0; i < n; i++) {
        y[i] = (i % 2); // etiquetas 0,1 alternadas
        for (int j = 0; j < d; j++)
            X[i * d + j] = (i + j) % 7 * 0.01;
    }

    double loss = 0.0;

    // ======== Bucle principal (versión secuencial) ========
    for (int i = 0; i < n; i++) {
        // z = w^T * x_i
        double z = 0.0;
        for (int j = 0; j < d; j++)
            z += w[j] * X[i * d + j];

        double p = sigmoid(z);
        double err = p - y[i];

        // Acumula pérdida
        loss += -(y[i] * log(p + 1e-12) + (1.0 - y[i]) * log(1.0 - p + 1e-12));

        // Acumula gradiente
        for (int j = 0; j < d; j++)
            grad[j] += err * X[i * d + j];
    }

    // Actualización de pesos (descenso del gradiente)
    double eta = 0.1 / n;
    for (int j = 0; j < d; j++)
        w[j] -= eta * grad[j];

    // Resultado
    cout << "Loss promedio = " << loss / n << endl;
    cout << "Primer peso w[0] = " << w[0] << endl;
}

