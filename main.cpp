//------------ EJERCICIOS DE DECODIFICACION CON CHAR------------------------------
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


// Convierte un byte en 8 chars '0'/'1'
void byteToBits(unsigned char byte, char *bits) {
    for (int i = 7; i >= 0; i--) {
        bits[7 - i] = ((byte >> i) & 1) ? '1' : '0';
    }
}

// Método 1 CORREGIDO: origBits = binario original, outBits = donde escribo el codificado
void encodeMethod1_fixed(const char *origBits, char *outBits, int totalBits, int n) {
    int bloques = totalBits / n;
    for (int b = 0; b < bloques; ++b) {
        const char *curOrig = origBits + b * n;
        char *curOut = outBits + b * n;

        if (b == 0) {
            // primer bloque: invertir todos los bits
            for (int i = 0; i < n; ++i) curOut[i] = (curOrig[i] == '0') ? '1' : '0';
            continue;
        }

        // contar 1s y 0s en el bloque anterior, ¡usando origBits!
        const char *prevOrig = origBits + (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i) {
            if (prevOrig[i] == '1') ++unos;
            else ++ceros;
        }

        if (unos == ceros) {
            // invertir todos
            for (int i = 0; i < n; ++i) curOut[i] = (curOrig[i] == '0') ? '1' : '0';
        } else if (ceros > unos) {
            // "invertir cada 2 bits" -> invertir cada segundo bit: índices 1,3,5,...
            for (int i = 0; i < n; ++i) {
                if (i % 2 == 1) curOut[i] = (curOrig[i] == '0') ? '1' : '0';
                else curOut[i] = curOrig[i];
            }
        } else { // unos > ceros
            // "invertir cada 3 bits" -> invertir cada tercer bit: índices 2,5,8,...
            for (int i = 0; i < n; ++i) {
                if (i % 3 == 2) curOut[i] = (curOrig[i] == '0') ? '1' : '0';
                else curOut[i] = curOrig[i];
            }
        }
    }
}

// Método 2 (usa orig -> out para mantener el mismo estilo)
void encodeMethod2_fixed(const char *origBits, char *outBits, int totalBits, int n) {
    int bloques = totalBits / n;
    for (int b = 0; b < bloques; ++b) {
        const char *curOrig = origBits + b * n;
        char *curOut = outBits + b * n;
        if (n <= 1) {
            if (n == 1) curOut[0] = curOrig[0];
            continue;
        }
        // rotación según enunciado: primer bit codificado = último del original,
        // segundo codificado = primero original, etc.
        curOut[0] = curOrig[n - 1];
        for (int i = 1; i < n; ++i) curOut[i] = curOrig[i - 1];
    }
}

int main() {
    // Ruta fija: reemplaza por tu ruta real (usa R"(...)" si hay espacios)
    const char *ruta = R"(C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/CARACTERES ASCII.txt)";

    int n, metodo;
    cout << "Ingrese semilla (n): ";
    cin >> n;
    cout << "Ingrese metodo (1 o 2): ";
    cin >> metodo;

    ifstream in(ruta, ios::binary);
    if (!in) {
        cerr << "No se pudo abrir el archivo en: " << ruta << "\n";
        return 1;
    }

    in.seekg(0, ios::end);
    streamsize size = in.tellg();
    in.seekg(0, ios::beg);

    if (size <= 0) {
        cerr << "Archivo vacío o tamaño inválido\n";
        return 1;
    }

    char *buffer = new char[size];
    in.read(buffer, size);
    in.close();

    int totalBits = static_cast<int>(size) * 8;
    int bloques = totalBits / n; // truncamos si no es múltiplo de n
    if (bloques == 0) {
        cerr << "Semilla n mayor que el número de bits del archivo\n";
        delete[] buffer;
        return 1;
    }
    int usableBits = bloques * n; // bits que sí se procesan

    // origBits contiene el binario sin codificar (solo los usableBits)
    char *origBits = new char[usableBits];
    for (int i = 0; i < size; ++i) byteToBits((unsigned char)buffer[i], origBits + i * 8);

    // outBits contendrá el código (solo usableBits)
    char *outBits = new char[usableBits];

    if (metodo == 1) {
        encodeMethod1_fixed(origBits, outBits, usableBits, n);
    } else if (metodo == 2) {
        encodeMethod2_fixed(origBits, outBits, usableBits, n);
    } else {
        cerr << "Metodo invalido\n";
        delete[] buffer; delete[] origBits; delete[] outBits;
        return 1;
    }

    cout << "\nBinario original (procesado, " << usableBits << " bits):\n";
    for (int i = 0; i < usableBits; ++i) cout << origBits[i];
    cout << "\n\nBinario codificado:\n";
    for (int i = 0; i < usableBits; ++i) cout << outBits[i];
    cout << "\n";

    // limpieza
    delete[] buffer;
    delete[] origBits;
    delete[] outBits;
    return 0;
}
