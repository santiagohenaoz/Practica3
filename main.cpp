#include <iostream>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <Declaraciones.h>
#include <stdexcept>


using namespace std;


//------------ EJERCICIOS DE DECODIFICACION CON CHAR------------------------------

/*
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

    try {
        ifstream in(ruta, ios::binary);
        if (!in) throw runtime_error("No se pudo abrir el archivo de entrada");

        in.seekg(0, ios::end);
        streamsize size = in.tellg();
        in.seekg(0, ios::beg);

        if (size <= 0) throw runtime_error("Archivo vacío o tamaño inválido");

        char *buffer = new char[size];
        in.read(buffer, size);
        in.close();

        int totalBits = static_cast<int>(size) * 8;
        int bloques = totalBits / n;
        if (bloques == 0) {
            delete[] buffer;
            throw runtime_error("Semilla n mayor que el número de bits del archivo");
        }
        int usableBits = bloques * n;

        char *origBits = new char[usableBits];
        for (int i = 0; i < size; ++i)
            byteToBits((unsigned char)buffer[i], origBits + i * 8);

        char *outBits = new char[usableBits];

        if (metodo == 1) {
            encodeMethod1_fixed(origBits, outBits, usableBits, n);
        } else if (metodo == 2) {
            encodeMethod2_fixed(origBits, outBits, usableBits, n);
        } else {
            delete[] buffer; delete[] origBits;
            throw runtime_error("Método inválido");
        }

        cout << "\nBinario original:\n";
        for (int i = 0; i < usableBits; ++i) cout << origBits[i];
        cout << "\n\nBinario codificado:\n";
        for (int i = 0; i < usableBits; ++i) cout << outBits[i];
        cout << "\n";

        int outBytes = usableBits / 8;
        unsigned char *outBuffer = new unsigned char[outBytes];

        for (int i = 0; i < outBytes; ++i) {
            unsigned char b = 0;
            for (int j = 0; j < 8; ++j) {
                b = (b << 1) | (outBits[i * 8 + j] - '0');
            }
            outBuffer[i] = b;
        }

        ofstream out(archivoSalida, ios::binary);
        if (!out) {
            delete[] buffer; delete[] origBits; delete[] outBits; delete[] outBuffer;
            throw runtime_error("No se pudo crear el archivo de salida");
        }
        out.write(reinterpret_cast<char*>(outBuffer), outBytes);
        out.close();

        cout << "Archivo binario codificado guardado en: " << archivoSalida << endl;

        // Limpieza
        delete[] buffer;
        delete[] origBits;
        delete[] outBits;
        delete[] outBuffer;
    }
    catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}



*/



//------------ EJERCICIO DE DECODIFICACION CON STRING------------------------------


/*

int main() {
    string Ruta, SalidaBinario;

    cout << "Ingrese la ruta del archivo de entrada: ";
    getline(cin, Ruta);

    cout << "Ingrese la ruta del archivo de salida: ";
    getline(cin, SalidaBinario);

    const char *nombre = Ruta.c_str();

    int n, metodo;
    cout << "EJERCICIO DE DECODIFICACION CON STRING" << endl;
    cout << "Ingrese semilla (n): "; cin >> n;
    cout << "Ingrese metodo (1 o 2): "; cin >> metodo;

    try {
        ifstream in(nombre, ios::binary);
        if (!in) throw runtime_error("No se pudo abrir el archivo de entrada");

        in.seekg(0, ios::end);
        streamsize sz = in.tellg();
        in.seekg(0, ios::beg);
        if (sz <= 0) throw runtime_error("El archivo está vacío o es inválido");

        string buffer((size_t)sz, '\0');
        in.read(&buffer[0], sz);
        in.close();

        // convertir a cadena de bits
        string bits;
        bits.reserve(buffer.size() * 8);
        for (unsigned char c : buffer)
            byteToBits(c, bits);

        int bloques = (int)bits.size() / n;
        if (bloques == 0) throw runtime_error("n es mayor que el número de bits disponibles");
        int usable = bloques * n;
        string orig = bits.substr(0, usable);

        cout << "\nBinario original:\n" << orig << "\n";

        string cod;
        if (metodo == 1) cod = encodeMethod1_fixed(orig, n);
        else if (metodo == 2) cod = encodeMethod2(orig, n);
        else throw runtime_error("Método inválido");

        cout << "\nBinario codificado:\n" << cod << "\n";

        // empaquetar bits en bytes reales
        int outBytes = cod.size() / 8;
        string outBuffer;
        outBuffer.reserve(outBytes);

        for (int i = 0; i < outBytes; i++) {
            unsigned char b = 0;
            for (int j = 0; j < 8; j++) {
                b = (b << 1) | (cod[i * 8 + j] - '0');
            }
            outBuffer.push_back(static_cast<char>(b));
        }

        // guardar archivo binario
        ofstream out(SalidaBinario, ios::binary);
        if (!out) throw runtime_error("No se pudo crear el archivo de salida");
        out.write(outBuffer.data(), outBuffer.size());
        out.close();

        cout << "\nArchivo binario codificado guardado en: " << SalidaBinario << endl;
    }
    catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

*/



// ------- PROGRAMA DECODIFICADOR CON CHAR---------------------------


/*
int main() {
    char rutaEntrada[260], rutaSalida[260];

    cout << "Ingrese la ruta del archivo codificado: ";
    cin.getline(rutaEntrada, 260);

    cout << "Ingrese la ruta del archivo de salida (decodificado): ";
    cin.getline(rutaSalida, 260);

    const char *archivoCodificado = rutaEntrada;

    int semilla, metodo;
    cout << "DECODIFICADOR CON CHAR" << endl;
    cout << "Ingrese semilla (n): ";
    cin >> semilla;
    cout << "Ingrese metodo (1 o 2): ";
    cin >> metodo;

    try {
        ifstream entrada(archivoCodificado, ios::binary);
        if (!entrada) throw runtime_error("No se pudo abrir el archivo de entrada codificado");

        entrada.seekg(0, ios::end);
        streamsize tamanoArchivo = entrada.tellg();
        entrada.seekg(0, ios::beg);

        if (tamanoArchivo <= 0) throw runtime_error("Archivo vacío o tamaño inválido");

        char *bufferEntrada = new char[tamanoArchivo];
        entrada.read(bufferEntrada, tamanoArchivo);
        entrada.close();

        int totalBits = static_cast<int>(tamanoArchivo) * 8;
        int bloques = totalBits / semilla;
        if (bloques == 0) {
            delete[] bufferEntrada;
            throw runtime_error("Semilla n mayor que el número de bits del archivo");
        }
        int bitsUtilizables = bloques * semilla;

        char *bitsCodificados = new char[bitsUtilizables];
        for (int i = 0; i < tamanoArchivo; ++i)
            byteToBits_dec((unsigned char)bufferEntrada[i], bitsCodificados + i * 8);

        char *bitsDecodificados = new char[bitsUtilizables];

        if (metodo == 1) {
            metodo1_decodificar(bitsCodificados, bitsDecodificados, bitsUtilizables, semilla);
        } else if (metodo == 2) {
            metodo2_decodificar(bitsCodificados, bitsDecodificados, bitsUtilizables, semilla);
        } else {
            delete[] bufferEntrada; delete[] bitsCodificados;
            throw runtime_error("Método inválido");
        }

        cout << "\nBinario decodificado:\n";
        for (int i = 0; i < bitsUtilizables; ++i) cout << bitsDecodificados[i];
        cout << "\n";

        int bytesSalida = bitsUtilizables / 8;
        unsigned char *bufferSalida = new unsigned char[bytesSalida];

        for (int i = 0; i < bytesSalida; ++i) {
            bufferSalida[i] = bitsToByte_dec(bitsDecodificados + i * 8);
        }

        ofstream salida(rutaSalida, ios::binary);
        if (!salida) {
            delete[] bufferEntrada; delete[] bitsCodificados; delete[] bitsDecodificados; delete[] bufferSalida;
            throw runtime_error("No se pudo crear el archivo de salida");
        }
        salida.write(reinterpret_cast<char*>(bufferSalida), bytesSalida);
        salida.close();

        cout << "Archivo decodificado guardado en: " << rutaSalida << endl;

        // Limpieza
        delete[] bufferEntrada;
        delete[] bitsCodificados;
        delete[] bitsDecodificados;
        delete[] bufferSalida;
    }
    catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}


*/



//----- PROGRAMA DECODIFICADOR CON STRING ---------------------------------------





int main() {
    try {
        // Rutas de entrada y salida
        string rutaEntrada, rutaSalida;
        cout << "Ingrese la ruta del archivo codificado: ";
        getline(cin, rutaEntrada);
        cout << "Ingrese la ruta del archivo de salida decodificado: ";
        getline(cin, rutaSalida);

        const char *nombre = rutaEntrada.c_str();

        int n, metodo;
        cout << "DECODIFICACION CON STRING" << endl;
        cout << "Ingrese semilla (n): ";
        if (!(cin >> n)) throw runtime_error("Error: semilla no valida.");
        cout << "Ingrese metodo (1 o 2): ";
        if (!(cin >> metodo)) throw runtime_error("Error: metodo no valido.");

        ifstream in(nombre, ios::binary);
        if (!in) throw runtime_error("No se pudo abrir el archivo de entrada.");

        in.seekg(0, ios::end);
        streamsize sz = in.tellg();
        in.seekg(0, ios::beg);
        if (sz <= 0) throw runtime_error("Archivo vacío o inválido.");

        string buffer((size_t)sz, '\0');
        in.read(&buffer[0], sz);
        in.close();

        // convertir bytes a bits
        string bits;
        bits.reserve(buffer.size() * 8);
        for (unsigned char c : buffer)
            convertirByteABits_string(c, bits);

        int bloques = (int)bits.size() / n;
        if (bloques == 0) throw runtime_error("La semilla n es mayor que el número de bits disponibles.");

        int usable = bloques * n;
        string usableBits = bits.substr(0, usable);

        cout << "\nBinario codificado:\n" << usableBits << "\n";

        string decoded;
        if (metodo == 1) decoded = decodificarMetodo1_string(usableBits, n);
        else if (metodo == 2) decoded = decodificarMetodo2_string(usableBits, n);
        else throw runtime_error("Método inválido. Debe ser 1 o 2.");

        cout << "\nBinario decodificado:\n" << decoded << "\n";

        // convertir bits a bytes
        string outBuffer = convertirBitsABytes_string(decoded);

        // guardar archivo decodificado
        ofstream out(rutaSalida, ios::binary);
        if (!out) throw runtime_error("No se pudo crear el archivo de salida.");
        out.write(outBuffer.data(), outBuffer.size());
        out.close();

        cout << "\nArchivo decodificado guardado en: " << rutaSalida << endl;
    }
    catch (const exception &e) {
        cerr << "Excepcion capturada: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "Error desconocido." << endl;
        return 1;
    }

    return 0;
}
