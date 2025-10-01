#include <fstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>


using namespace std;


//------------ EJERCICIOS DE DECODIFICACION CON CHAR------------------------------

// Convierte un byte en 8 chars '0'/'1'
void byteToBits(unsigned char byte, char *bits) {
    for (int i = 7; i >= 0; i--) {
        bits[7 - i] = ((byte >> i) & 1) ? '1' : '0';
    }
}


// Metodo 1 : origBits = binario original, outBits = donde escribo el codificado
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




// Método 2 (use orig -> out para mantenerlo claro)
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



//------------ EJERCICIOS DE DECODIFICACION CON STRING------------------------------

// Convierte un byte en 8 caracteres '0'/'1' y los agrega al string
void byteToBits(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits.push_back(((byte >> i) & 1) ? '1' : '0');
}

// Método 1 : siempre lee el bloque anterior del ORIGINAL
string encodeMethod1_fixed(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int startCur = b * n;
        if (b == 0) {
            // primer bloque: invertir todos
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
            continue;
        }

        int startPrev = (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i)
            (bits[startPrev + i] == '1') ? ++unos : ++ceros;

        if (unos == ceros) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
        }
        else if (ceros > unos) {
            // invertir cada 2 bits -> posiciones impares
            for (int i = 0; i < n; ++i) {
                if (i % 2 == 1)
                    out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else
                    out[startCur + i] = bits[startCur + i];
            }
        }
        else { // unos > ceros
            // invertir cada 3 bits -> posiciones 2,5,8,... (i%3==2)
            for (int i = 0; i < n; ++i) {
                if (i % 3 == 2)
                    out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else
                    out[startCur + i] = bits[startCur + i];
            }
        }
    }
    return out;
}

// Método 2 (rotación por bloque)
string encodeMethod2(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    string out;
    out.resize(bloques * n);
    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) { out[start] = bits[start]; continue; }
        out[start + 0] = bits[start + n - 1];
        for (int i = 1; i < n; ++i)
            out[start + i] = bits[start + i - 1];
    }
    return out;
}
