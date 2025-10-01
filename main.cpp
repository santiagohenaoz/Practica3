#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <Declaraciones.h>

using namespace std;


//------------ EJERCICIOS DE DECODIFICACION CON CHAR------------------------------

int main() {
    char Ruta[260], archivoSalida[260];

    cout << "Ingrese la ruta del archivo de entrada: ";
    cin.getline(Ruta, 260);


    cout << "Ingrese la ruta del archivo de salida: ";
    cin.getline(archivoSalida, 260);

    const char *ruta = Ruta;

    int n, metodo;
    cout << "EJERCICIO DE DECODIFICACION CON CHAR" << endl;
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

    cout << "\nBinario original:\n";
    for (int i = 0; i < usableBits; ++i) cout << origBits[i];
    cout << "\n\nBinario codificado:\n";
    for (int i = 0; i < usableBits; ++i) cout << outBits[i];
    cout << "\n";

    // limpieza
    delete[] buffer;
    delete[] origBits;
    delete[] outBits;



    int outBytes = usableBits / 8;
    unsigned char *outBuffer = new unsigned char[outBytes];

    for (int i = 0; i < outBytes; ++i) {
        unsigned char b = 0;
        for (int j = 0; j < 8; ++j) {
            b = (b << 1) | (outBits[i * 8 + j] - '0');
        }
        outBuffer[i] = b;
    }

    // Escribir archivo de salida en binario
    ofstream out(archivoSalida, ios::binary);
    if (!out) {
        cerr << "No se pudo crear el archivo de salida\n";
        delete[] buffer; delete[] origBits; delete[] outBits; delete[] outBuffer;
        return 1;
    }
    out.write(reinterpret_cast<char*>(outBuffer), outBytes);
    out.close();

    cout << "Archivo binario codificado guardado en: " << archivoSalida << endl;

    // limpieza extra
    delete[] outBuffer;

    return 0;
}




//------------ EJERCICIO DE DECODIFICACION CON STRING------------------------------


/*
int main() {
    // Ruta del  archivo
    string Ruta;
    cout << "Ingrese la ruta del archivo: ";
    getline (cin, Ruta);

    const char *nombre = Ruta.c_str();

    int n, metodo;
    cout << "EJERCICIO DE DECODIFICACION CON STRING" << endl;
    cout << "Ingrese semilla (n): "; cin >> n;
    cout << "Ingrese metodo (1 o 2): "; cin >> metodo;

    ifstream in(nombre, ios::binary);
    if (!in) { cerr << "No se pudo abrir: " << nombre << "\n"; return 1; }
    in.seekg(0, ios::end);
    streamsize sz = in.tellg();
    in.seekg(0, ios::beg);
    if (sz <= 0) { cerr << "Archivo vacío o inválido\n"; return 1; }

    string buffer((size_t)sz, '\0');
    in.read(&buffer[0], sz);
    in.close();

    // convertir a cadena de bits
    string bits;
    bits.reserve(buffer.size() * 8);
    for (unsigned char c : buffer)
        byteToBits(c, bits);

    int bloques = (int)bits.size() / n;
    if (bloques == 0) { cerr << "n es mayor que el número de bits disponibles\n"; return 1; }
    int usable = bloques * n;
    string orig = bits.substr(0, usable);

    cout << "\nBinario original:\n" << orig << "\n";

    string cod;
    if (metodo == 1) cod = encodeMethod1_fixed(orig, n);
    else if (metodo == 2) cod = encodeMethod2(orig, n);
    else { cerr << "Método inválido\n"; return 1; }

    cout << "\nBinario codificado:\n" << cod << "\n";
    return 0;
}


*/
