#include <fstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>


using namespace std;


//------------ EJERCICIOS DE CODIFICACION CON CHAR------------------------------

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
            // "invertir cada 2 bits" -> invertir cada segundo bit: indices 1,3,5,...
            for (int i = 0; i < n; ++i) {
                if (i % 2 == 1) curOut[i] = (curOrig[i] == '0') ? '1' : '0';
                else curOut[i] = curOrig[i];
            }
        } else { // unos > ceros
            // "invertir cada 3 bits" -> invertir cada tercer bit: indices 2,5,8,...
            for (int i = 0; i < n; ++i) {
                if (i % 3 == 2) curOut[i] = (curOrig[i] == '0') ? '1' : '0';
                else curOut[i] = curOrig[i];
            }
        }
    }
}




// Metodo 2 (use orig -> out para mantenerlo claro)
void encodeMethod2_fixed(const char *origBits, char *outBits, int totalBits, int n) {
    int bloques = totalBits / n;
    for (int b = 0; b < bloques; ++b) {
        const char *curOrig = origBits + b * n;
        char *curOut = outBits + b * n;
        if (n <= 1) {
            if (n == 1) curOut[0] = curOrig[0];
            continue;
        }
        // rotacion segun enunciado: primer bit codificado = ultimo del original,
        // segundo codificado = primero original y asi.
        curOut[0] = curOrig[n - 1];
        for (int i = 1; i < n; ++i) curOut[i] = curOrig[i - 1];
    }
}



//------------ EJERCICIOS DE CODIFICACION CON STRING------------------------------

// Convierte un byte en 8 caracteres '0'/'1' y los agrega al string
void byteToBits(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits.push_back(((byte >> i) & 1) ? '1' : '0');
}

// Metodo 1 : siempre lee el bloque anterior del ORIGINAL
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

// Método 2 (rotacion por bloque)
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




//----- PROGRAMA DECODIFICADOR CON CHAR ---------------------------------------


// Funcion auxiliar: pasa un byte a bits
void byteToBits_dec(unsigned char byte, char *bitArray) {
    for (int i = 7; i >= 0; --i) {
        bitArray[7 - i] = ((byte >> i) & 1) ? '1' : '0';
    }
}

// Reconstruye un byte desde 8 chars '0'/'1'
unsigned char bitsToByte_dec(const char *bitArray) {
    unsigned char b = 0;
    for (int i = 0; i < 8; ++i) {
        b = (b << 1) | (bitArray[i] - '0');
    }
    return b;
}

// ----------------------
// Metodos inversos de decodificacion
void metodo1_decodificar(const char *encodedBits, char *decodedBits, int totalBits, int n) {
    int bloques = totalBits / n;

    for (int b = 0; b < bloques; ++b) {
        const char *curEnc = encodedBits + b * n;
        char *curDec = decodedBits + b * n;

        if (b == 0) {
            // Primer bloque: en codificaci0n se invirti0 todo → aqui invertimos todo otra vez
            for (int i = 0; i < n; ++i)
                curDec[i] = (curEnc[i] == '0') ? '1' : '0';
            continue;
        }

        // Contamos 1s y 0s en el bloque anterior ya decodificado
        const char *prevDec = decodedBits + (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i) {
            if (prevDec[i] == '1') ++unos;
            else ++ceros;
        }

        if (unos == ceros) {
            // Se invirtio todo → invertimos todo otra vez
            for (int i = 0; i < n; ++i)
                curDec[i] = (curEnc[i] == '0') ? '1' : '0';
        } else if (ceros > unos) {
            // Se invirtieron los indices 1,3,5... → invertimos otra vez esos indices
            for (int i = 0; i < n; ++i) {
                if (i % 2 == 1) curDec[i] = (curEnc[i] == '0') ? '1' : '0';
                else curDec[i] = curEnc[i];
            }
        } else { // unos > ceros
            // Se invirtieron los indices 2,5,8... → invertimos otra vez esos indices
            for (int i = 0; i < n; ++i) {
                if (i % 3 == 2) curDec[i] = (curEnc[i] == '0') ? '1' : '0';
                else curDec[i] = curEnc[i];
            }
        }
    }
}


void metodo2_decodificar(const char *entradaBits, char *salidaBits, int totalBits, int n) {
    for (int i = 0; i < totalBits; i += n) {
        // inverso del metodo 2: rotar a la izquierda
        for (int j = 0; j < n; ++j) {
            salidaBits[i + j] = entradaBits[i + ((j + 1) % n)];
        }
    }
}




//----- PROGRAMA DECODIFICADOR CON STRING ---------------------------------------


// Convierte un byte en 8 caracteres '0'/'1' y los agrega al string
void convertirByteABits_string(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits.push_back(((byte >> i) & 1) ? '1' : '0');
}

// Método 1: decodificación con string
string decodificarMetodo1_string(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int startCur = b * n;
        if (b == 0) {
            // primer bloque: fue invertido totalmente → lo invertimos otra vez
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
            continue;
        }

        // contar unos y ceros en el bloque anterior ya decodificado
        int startPrev = (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i)
            (out[startPrev + i] == '1') ? ++unos : ++ceros;

        if (unos == ceros) {
            // todo se invirtió → invertimos todo otra vez
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
        }
        else if (ceros > unos) {
            // se invirtieron posiciones impares (1,3,5,...) → invertimos otra vez esas
            for (int i = 0; i < n; ++i) {
                if (i % 2 == 1)
                    out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else
                    out[startCur + i] = bits[startCur + i];
            }
        }
        else { // unos > ceros
            // se invirtieron posiciones 2,5,8... → invertimos otra vez esas
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

// Metodo 2: decodificacion con string (rotacion inversa)
string decodificarMetodo2_string(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) {
            out[start] = bits[start];
            continue;
        }
        // la codificacion rotaba a la derecha → para decodificar rotamos a la izquierda
        for (int i = 0; i < n - 1; ++i)
            out[start + i] = bits[start + i + 1];
        out[start + n - 1] = bits[start];
    }
    return out;
}

// Convierte string de '0'/'1' a bytes reales
string convertirBitsABytes_string(const string &bits) {
    int outBytes = bits.size() / 8;
    string outBuffer;
    outBuffer.reserve(outBytes);
    for (int i = 0; i < outBytes; i++) {
        unsigned char b = 0;
        for (int j = 0; j < 8; j++) {
            b = (b << 1) | (bits[i * 8 + j] - '0');
        }
        outBuffer.push_back(static_cast<char>(b));
    }
    return outBuffer;
}


//---------------APLICACION-----------------------------------------------------------------------


void convertirByteABits_aplicacion(unsigned char byte, string &bits) {
    for (int i = 7; i >= 0; --i)
        bits.push_back(((byte >> i) & 1) ? '1' : '0');
}

string convertirBitsABytes_aplicacion(const string &bits) {
    int outBytes = bits.size() / 8;
    string outBuffer;
    outBuffer.reserve(outBytes);

    for (int i = 0; i < outBytes; i++) {
        unsigned char b = 0;
        for (int j = 0; j < 8; j++) {
            b = (b << 1) | (bits[i * 8 + j] - '0');
        }
        outBuffer.push_back(static_cast<char>(b));
    }
    return outBuffer;
}

string codificarMetodo1_string(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int startCur = b * n;
        if (b == 0) {
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
        } else if (ceros > unos) {
            for (int i = 0; i < n; ++i)
                if (i % 2 == 1) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        } else {
            for (int i = 0; i < n; ++i)
                if (i % 3 == 2) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        }
    }
    return out;
}


// Decodificar Metodo 1
string decodificarMetodo1_aplicacion(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    if (bloques == 0) return string();

    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int startCur = b * n;
        if (b == 0) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
            continue;
        }
        int startPrev = (b - 1) * n;
        int unos = 0, ceros = 0;
        for (int i = 0; i < n; ++i)
            (out[startPrev + i] == '1') ? ++unos : ++ceros;

        if (unos == ceros) {
            for (int i = 0; i < n; ++i)
                out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
        } else if (ceros > unos) {
            for (int i = 0; i < n; ++i)
                if (i % 2 == 1) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        } else {
            for (int i = 0; i < n; ++i)
                if (i % 3 == 2) out[startCur + i] = (bits[startCur + i] == '0') ? '1' : '0';
                else out[startCur + i] = bits[startCur + i];
        }
    }
    return out;
}

// Codificar Metodo 2
string codificarMetodo2_string(const string &bits, int n) {
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

// Decodificar Metodo 2
string decodificarMetodo2_aplicacion(const string &bits, int n) {
    int totalBits = (int)bits.size();
    int bloques = totalBits / n;
    string out;
    out.resize(bloques * n);

    for (int b = 0; b < bloques; ++b) {
        int start = b * n;
        if (n == 1) { out[start] = bits[start]; continue; }
        for (int i = 0; i < n - 1; ++i)
            out[start + i] = bits[start + i + 1];
        out[start + n - 1] = bits[start];
    }
    return out;
}

//sistema de archivos
string leerArchivoBinario(const string &ruta) {
    ifstream in(ruta, ios::binary);
    if (!in) throw runtime_error("No se pudo abrir archivo: " + ruta);
    in.seekg(0, ios::end);
    streamsize sz = in.tellg();
    in.seekg(0, ios::beg);
    string buffer((size_t)sz, '\0');
    in.read(&buffer[0], sz);
    return buffer;
}

void guardarArchivoBinario(const string &ruta, const string &data) {
    ofstream out(ruta, ios::binary);
    if (!out) throw runtime_error("No se pudo guardar archivo: " + ruta);
    out.write(data.data(), data.size());
}

