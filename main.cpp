#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include "Declaraciones.h"

using namespace std;

int main() {
    bool salir = false;

    while (!salir) {


        int opcionPrincipal;
        cout << "\n------- MENU PRINCIPAL --------\n";
        cout << "1. Codificacion con CHAR\n";
        cout << "2. Codificacion con STRING\n";
        cout << "3. Decodificacion con CHAR\n";
        cout << "4. Decodificacion con STRING\n";
        cout << "5. Aplicacion STRING (Administrador / Usuario)\n";
        cout << "6. Aplicacion CHAR (Administrador / Usuario)\n";
        cout << "7. Salir\n";
        cout << "-------------------------\n";
        cout << "Seleccione una opcion: ";
        cin >> opcionPrincipal;
        cin.ignore();

        switch (opcionPrincipal) {
        case 1: {
            cout << "\n-- CODIFICACION CON CHAR --\n";

            char Ruta[260], archivoSalida[260];

            cout << "Ingrese la ruta del archivo de entrada: ";
            cin.getline(Ruta, 260);

            cout << "Ingrese la ruta del archivo de salida: ";
            cin.getline(archivoSalida, 260);

            const char *ruta = Ruta;

            int n, metodo;
            cout << "EJERCICIO DE CODIFICACION CON CHAR" << endl;
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


            break;
        }

        case 2: {

            cout << "\n-- CODIFICACION CON STRING --\n";

            string rutaEntrada, rutaSalida;
            cout << "Ingrese la ruta del archivo de entrada: ";
            getline(cin, rutaEntrada);

            cout << "Ingrese la ruta del archivo de salida: ";
            getline(cin, rutaSalida);

            int n, metodo;
            cout << "\nEJERCICIO DE CODIFICACION CON STRING\n";
            cout << "Ingrese semilla (n): ";
            cin >> n;
            cout << "Ingrese metodo (1 o 2): ";
            cin >> metodo;

            try {
                // Abrir archivo de entrada
                ifstream in(rutaEntrada, ios::binary);
                if (!in) throw runtime_error("No se pudo abrir el archivo de entrada");

                // Obtener tamaño del archivo
                in.seekg(0, ios::end);
                streamsize sz = in.tellg();
                in.seekg(0, ios::beg);
                if (sz <= 0) throw runtime_error("El archivo está vacío o inválido");

                // Leer contenido del archivo
                string buffer(sz, '\0');
                in.read(&buffer[0], sz);
                in.close();

                // Convertir bytes a cadena de bits
                string bits;
                bits.reserve(buffer.size() * 8);
                for (unsigned char c : buffer)
                    byteToBits(c, bits);

                // Calcular bloques válidos
                int bloques = bits.size() / n;
                if (bloques == 0) throw runtime_error("n es mayor que el número de bits disponibles");

                int usable = bloques * n;
                string orig = bits.substr(0, usable);

                cout << "\nBinario original:\n" << orig << "\n";

                // Aplicar método de codificación
                string cod;
                if (metodo == 1) cod = encodeMethod1_fixed(orig, n);
                else if (metodo == 2) cod = encodeMethod2(orig, n);
                else throw runtime_error("Método inválido. Debe ser 1 o 2.");

                cout << "\nBinario codificado:\n" << cod << "\n";

                // Empaquetar bits en bytes
                int outBytes = cod.size() / 8;
                string outBuffer;
                outBuffer.reserve(outBytes);

                for (int i = 0; i < outBytes; ++i) {
                    unsigned char b = 0;
                    for (int j = 0; j < 8; ++j)
                        b = (b << 1) | (cod[i * 8 + j] - '0');
                    outBuffer.push_back(static_cast<char>(b));
                }

                // Guardar archivo de salida
                ofstream out(rutaSalida, ios::binary);
                if (!out) throw runtime_error("No se pudo crear el archivo de salida");
                out.write(outBuffer.data(), outBuffer.size());
                out.close();

                cout << "\nArchivo codificado guardado en: " << rutaSalida << endl;
            }
            catch (const exception &e) {
                cerr << "Error: " << e.what() << endl;
                return 1;
            }




            break;
        }

        case 3: {
            cout << "\n-- DECODIFICACION CON CHAR --\n";



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


            break;
        }

        case 4: {

            try {
                cout << "\n-- DECODIFICACION CON STRING --\n";

                string rutaEntrada, rutaSalida;
                cout << "Ingrese la ruta del archivo codificado: ";
                getline(cin, rutaEntrada);
                cout << "Ingrese la ruta del archivo de salida decodificado: ";
                getline(cin, rutaSalida);

                int n, metodo;
                cout << "Ingrese semilla (n): ";
                cin >> n;
                cout << "Ingrese metodo (1 o 2): ";
                cin >> metodo;

                // Leer archivo binario codificado
                ifstream in(rutaEntrada, ios::binary);
                if (!in) throw runtime_error("No se pudo abrir el archivo de entrada.");

                in.seekg(0, ios::end);
                streamsize sz = in.tellg();
                in.seekg(0, ios::beg);
                if (sz <= 0) throw runtime_error("Archivo vacío o inválido.");

                string buffer((size_t)sz, '\0');
                in.read(&buffer[0], sz);
                in.close();

                // Convertir bytes a bits
                string bits;
                bits.reserve(buffer.size() * 8);
                for (unsigned char c : buffer)
                    byteToBits_dec(c, bits);

                // Decodificar según método
                string decoded;
                if (metodo == 1)
                    decoded = decodeMethod1(bits, n);
                else if (metodo == 2)
                    decoded = decodeMethod2(bits, n);
                else
                    throw runtime_error("Método inválido. Debe ser 1 o 2.");

                cout << "\nBits decodificados:\n" << decoded << "\n";

                // Convertir bits a bytes (texto original)
                string outBuffer;
                int outBytes = decoded.size() / 8;
                outBuffer.reserve(outBytes);
                for (int i = 0; i < outBytes; ++i)
                    outBuffer += static_cast<char>(bitsToByte_dec(decoded, i * 8));

                // Guardar archivo decodificado
                ofstream out(rutaSalida, ios::binary);
                if (!out) throw runtime_error("No se pudo crear el archivo de salida.");
                out.write(outBuffer.data(), outBuffer.size());
                out.close();

                cout << "\nArchivo decodificado guardado en: " << rutaSalida << endl;
            }
            catch (const exception &e) {
                cerr << "Error: " << e.what() << endl;
                return 1;
            }

            break;
        }

        case 5: {
            cout << "\n-- MODO APLICACION --\n";

            try {
                int modo = 0;
                char continuar = 's';

                do {
                    cout << "Seleccione modo:\n"
                         << "1. Super Administrador\n"
                         << "2. Administrador\n"
                         << "3. Usuario\n"
                         << "4. Salir al menu principal\n> ";
                    cin >> modo;
                    cin.ignore();


                    // MODO SUPER ADMINISTRADOR

                    if (modo == 1) {
                        try {
                            cout << "Ingrese clave del SUPER ADMIN: ";
                            string claveInput; getline(cin, claveInput);
                            if (claveInput.empty()) throw runtime_error("La clave no puede estar vacia.");

                            ifstream test("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/SUPERADMIN.txt");
                            if (!test.is_open()) throw runtime_error("Archivo SUPERADMIN.txt no encontrado.");
                            test.close();

                            string superCod = leerArchivoBinario("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/SUPERADMIN.txt");
                            if (superCod.empty()) throw runtime_error("El archivo SUPERADMIN.txt está vacío.");

                            string bits;
                            for (unsigned char c : superCod)
                                convertirByteABits_aplicacion(c, bits);

                            bool acceso = false;
                            int nEncontrado = 0, metodoEncontrado = 0;
                            string claveReal;

                            for (int n = 1; n <= 7 && !acceso; ++n) {
                                for (int metodo = 1; metodo <= 2 && !acceso; ++metodo) {
                                    string decod = (metodo == 1)
                                    ? decodificarMetodo1_aplicacion(bits, n)
                                    : decodificarMetodo2_aplicacion(bits, n);
                                    string claveRealTmp = convertirBitsABytes_aplicacion(decod);
                                    if (claveInput == claveRealTmp) {
                                        acceso = true;
                                        nEncontrado = n;
                                        metodoEncontrado = metodo;
                                        claveReal = claveRealTmp;
                                    }
                                }
                            }

                            if (!acceso) throw runtime_error("Clave incorrecta o no coincide con el archivo.");

                            cout << "Acceso concedido como SUPER ADMIN.\n";
                            cout << "Desea registrar un nuevo ADMIN? (s/n): ";
                            char op; cin >> op; cin.ignore();

                            if (op == 's' || op == 'S') {
                                string cedula, clave;
                                cout << "Ingrese cedula del nuevo admin: ";
                                getline(cin, cedula);
                                cout << "Ingrese clave: ";
                                getline(cin, clave);
                                if (cedula.empty() || clave.empty())
                                    throw runtime_error("Ni la cedula ni la clave pueden estar vacias.");

                                int nAdmin, metodoAdmin;
                                cout << "Ingrese semilla (n) para este admin: ";
                                cin >> nAdmin; cin.ignore();
                                cout << "Ingrese metodo (1 o 2) para este admin: ";
                                cin >> metodoAdmin; cin.ignore();

                                if (nAdmin < 1 || nAdmin > 7) throw runtime_error("Semilla fuera de rango (1-7).");
                                if (metodoAdmin != 1 && metodoAdmin != 2) throw runtime_error("Metodo invalido (solo 1 o 2).");

                                string bitsAdmin;
                                for (unsigned char c : clave)
                                    convertirByteABits_aplicacion(c, bitsAdmin);

                                string cod;
                                if (metodoAdmin == 1) cod = codificarMetodo1_string(bitsAdmin, nAdmin);
                                else cod = codificarMetodo2_string(bitsAdmin, nAdmin);

                                string claveCod = convertirBitsABytes_aplicacion(cod);

                                ofstream out("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/administradores.txt", ios::app);
                                if (!out.is_open()) throw runtime_error("No se pudo abrir administradores.txt para escribir.");
                                out << cedula << "," << claveCod << "," << nAdmin << "," << metodoAdmin << "\n";
                                out.close();

                                cout << "Administrador registrado exitosamente con n="
                                     << nAdmin << " y metodo=" << metodoAdmin << ".\n";
                            }
                        } catch (const exception &e) {
                            cerr << "Error en modo SUPER ADMIN: " << e.what() << "\n";
                        }
                    }


                    // MODO ADMINISTRADOR

                    else if (modo == 2) {
                        try {
                            cout << "Ingrese cedula de admin: ";
                            string cedulaAdmin; getline(cin, cedulaAdmin);
                            cout << "Ingrese clave: ";
                            string clave; getline(cin, clave);
                            if (cedulaAdmin.empty() || clave.empty())
                                throw runtime_error("Debe ingresar cedula y clave.");

                            ifstream in("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/administradores.txt");
                            if (!in) throw runtime_error("No se encontro administradores.txt");

                            bool valido = false;
                            int nAdmin = 0, metodoAdmin = 0;
                            string linea;
                            while (getline(in, linea)) {
                                stringstream ss(linea);
                                string cedulaF, claveF, nF, metodoF;
                                getline(ss, cedulaF, ',');
                                getline(ss, claveF, ',');
                                getline(ss, nF, ',');
                                getline(ss, metodoF, ',');

                                if (cedulaF == cedulaAdmin) {
                                    nAdmin = stoi(nF);
                                    metodoAdmin = stoi(metodoF);

                                    string bits;
                                    for (unsigned char c : claveF)
                                        convertirByteABits_aplicacion(c, bits);

                                    string claveDec;
                                    if (metodoAdmin == 1) claveDec = decodificarMetodo1_aplicacion(bits, nAdmin);
                                    else claveDec = decodificarMetodo2_aplicacion(bits, nAdmin);

                                    string claveReal = convertirBitsABytes_aplicacion(claveDec);

                                    if (claveReal == clave) valido = true;
                                    break;
                                }
                            }
                            in.close();

                            if (!valido) throw runtime_error("Admin no autorizado o clave incorrecta.");

                            cout << "Acceso como ADMINISTRADOR.\n";
                            cout << "Desea registrar un nuevo usuario? (s/n): ";
                            char op; cin >> op; cin.ignore();

                            if (op == 's' || op == 'S') {
                                string cedula, clave, saldo;
                                cout << "Ingrese cedula del usuario: ";
                                getline(cin, cedula);
                                cout << "Ingrese clave: ";
                                getline(cin, clave);
                                cout << "Ingrese saldo inicial: ";
                                getline(cin, saldo);

                                if (cedula.empty() || clave.empty() || saldo.empty())
                                    throw runtime_error("Campos vacíos al registrar usuario.");

                                int nUser, metodoUser;
                                cout << "Ingrese semilla (n) para el usuario: ";
                                cin >> nUser; cin.ignore();
                                cout << "Ingrese metodo (1 o 2): ";
                                cin >> metodoUser; cin.ignore();

                                if (nUser < 1 || nUser > 7) throw runtime_error("Semilla fuera de rango (1-7).");
                                if (metodoUser != 1 && metodoUser != 2) throw runtime_error("Método invalido (solo 1 o 2).");

                                string bits;
                                for (unsigned char c : clave)
                                    convertirByteABits_aplicacion(c, bits);

                                string cod;
                                if (metodoUser == 1) cod = codificarMetodo1_string(bits, nUser);
                                else cod = codificarMetodo2_string(bits, nUser);

                                string claveCod = convertirBitsABytes_aplicacion(cod);

                                ofstream out("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/usuarios.txt", ios::app);
                                if (!out.is_open()) throw runtime_error("No se pudo abrir usuarios.txt para escribir.");
                                out << cedula << "," << claveCod << "," << saldo
                                    << "," << nUser << "," << metodoUser << "\n";
                                out.close();

                                cout << "Usuario registrado correctamente.\n";
                            }
                        } catch (const exception &e) {
                            cerr << "Error en modo ADMIN: " << e.what() << "\n";
                        }
                    }


                    // MODO USUARIO

                    else if (modo == 3) {
                        try {
                            cout << "Ingrese cedula: ";
                            string cedula; getline(cin, cedula);
                            cout << "Ingrese clave: ";
                            string clave; getline(cin, clave);
                            if (cedula.empty() || clave.empty())
                                throw runtime_error("Debe ingresar cedula y clave.");

                            ifstream in("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/usuarios.txt");
                            if (!in) throw runtime_error("No existe usuarios.txt");

                            bool encontrado = false;
                            string linea;
                            while (getline(in, linea)) {
                                stringstream ss(linea);
                                string cedulaF, claveF, saldoF, nF, metodoF;
                                getline(ss, cedulaF, ',');
                                getline(ss, claveF, ',');
                                getline(ss, saldoF, ',');
                                getline(ss, nF, ',');
                                getline(ss, metodoF, ',');

                                if (cedula == cedulaF) {
                                    int nUser = stoi(nF);
                                    int metodoUser = stoi(metodoF);

                                    string bits;
                                    for (unsigned char c : claveF)
                                        convertirByteABits_aplicacion(c, bits);

                                    string claveDec;
                                    if (metodoUser == 1) claveDec = decodificarMetodo1_aplicacion(bits, nUser);
                                    else claveDec = decodificarMetodo2_aplicacion(bits, nUser);

                                    string claveReal = convertirBitsABytes_aplicacion(claveDec);

                                    if (claveReal == clave) {
                                        encontrado = true;
                                        int saldo = stoi(saldoF);
                                        int opcion;
                                        do {
                                            cout << "\n1. Consultar saldo\n2. Retirar dinero\n3. Salir\n> ";
                                            cin >> opcion;

                                            if (cin.fail()) {
                                                cin.clear();
                                                cin.ignore(1000, '\n');
                                                throw runtime_error("Entrada no valida. Ingrese un número.");
                                            }

                                            if (opcion == 1) {
                                                saldo -= 1000;
                                                cout << "Saldo actual: " << saldo << "\n";
                                            } else if (opcion == 2) {
                                                int retiro;
                                                cout << "Monto a retirar: ";
                                                cin >> retiro;
                                                if (cin.fail() || retiro <= 0)
                                                    throw runtime_error("Monto invalido.");
                                                if (retiro + 1000 <= saldo) {
                                                    saldo -= retiro + 1000;
                                                    cout << "Retiro exitoso. Nuevo saldo: " << saldo << "\n";
                                                } else {
                                                    cout << "Fondos insuficientes.\n";
                                                }
                                            }
                                        } while (opcion != 3);
                                    }
                                }
                            }

                            in.close();

                            if (!encontrado)
                                throw runtime_error("Usuario o clave incorrecta.");
                        } catch (const exception &e) {
                            cerr << "Error en modo USUARIO: " << e.what() << "\n";
                        }
                    }

                    else if (modo == 4) {
                        cout << "Regresando al menu principal...\n";
                        break;
                    }
                    else {
                        cout << "Opcion invalida.\n";
                    }

                    cout << "\nDesea volver al menu de modos? (s/n): ";
                    cin >> continuar;
                    cin.ignore();

                } while (continuar == 's' || continuar == 'S');

            } catch (const exception &e) {
                cerr << "Error general: " << e.what() << "\n";
            }


            break;
        }

        case 6:{

            cout << "\n== MODO APLICACION ==\n";

            try {
                int modo = 0;
                char continuar = 's';

                do {
                    cout << "Seleccione modo:\n"
                         << "1. Super Administrador\n"
                         << "2. Administrador\n"
                         << "3. Usuario\n"
                         << "4. Salir al menu principal\n> ";
                    cin >> modo;
                    cin.ignore();


                    // MODO SUPER ADMINISTRADOR

                    if (modo == 1) {
                        try {
                            char claveInput[100];
                            cout << "Ingrese clave del SUPER ADMIN: ";
                            cin.getline(claveInput, 100);
                            if (strlen(claveInput) == 0) throw runtime_error("La clave no puede estar vacia.");

                            ifstream test("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/SUPERADMIN.txt", ios::binary);
                            if (!test.is_open()) throw runtime_error("Archivo SUPERADMIN.txt no encontrado.");
                            test.close();

                            string superCod = leerArchivoBinario("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/SUPERADMIN.txt");
                            if (superCod.empty()) throw runtime_error("El archivo SUPERADMIN.txt esta vacio.");

                            string bits;
                            for (unsigned char c : superCod)
                                convertirByteABits_aplicacion(c, bits);

                            bool acceso = false;
                            int nEncontrado = 0, metodoEncontrado = 0;
                            string claveReal;

                            for (int n = 1; n <= 7 && !acceso; ++n) {
                                for (int metodo = 1; metodo <= 2 && !acceso; ++metodo) {
                                    string decod = (metodo == 1)
                                    ? decodificarMetodo1_aplicacion(bits, n)
                                    : decodificarMetodo2_aplicacion(bits, n);
                                    string claveRealTmp = convertirBitsABytes_aplicacion(decod);
                                    if (strcmp(claveInput, claveRealTmp.c_str()) == 0) {
                                        acceso = true;
                                        nEncontrado = n;
                                        metodoEncontrado = metodo;
                                        claveReal = claveRealTmp;
                                    }
                                }
                            }

                            if (!acceso) throw runtime_error("Clave incorrecta o no coincide con el archivo.");

                            cout << "Acceso concedido como SUPER ADMIN.\n";
                            cout << "Desea registrar un nuevo ADMIN? (s/n): ";
                            char op; cin >> op; cin.ignore();

                            if (op == 's' || op == 'S') {
                                char cedula[50], clave[50];
                                cout << "Ingrese cedula del nuevo admin: ";
                                cin.getline(cedula, 50);
                                cout << "Ingrese clave: ";
                                cin.getline(clave, 50);

                                if (strlen(cedula) == 0 || strlen(clave) == 0)
                                    throw runtime_error("Ni la cedula ni la clave pueden estar vacias.");

                                int nAdmin, metodoAdmin;
                                cout << "Ingrese semilla (n) para este admin: ";
                                cin >> nAdmin; cin.ignore();
                                cout << "Ingrese metodo (1 o 2): ";
                                cin >> metodoAdmin; cin.ignore();

                                if (nAdmin < 1 || nAdmin > 7) throw runtime_error("Semilla fuera de rango (1-7).");
                                if (metodoAdmin != 1 && metodoAdmin != 2) throw runtime_error("Método inválido (solo 1 o 2).");

                                string bitsAdmin;
                                for (int i = 0; clave[i] != '\0'; ++i)
                                    convertirByteABits_aplicacion((unsigned char)clave[i], bitsAdmin);

                                string cod = (metodoAdmin == 1)
                                                 ? codificarMetodo1_string(bitsAdmin, nAdmin)
                                                 : codificarMetodo2_string(bitsAdmin, nAdmin);

                                string claveCod = convertirBitsABytes_aplicacion(cod);

                                ofstream out("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/administradores.txt", ios::app);
                                if (!out.is_open()) throw runtime_error("No se pudo abrir administradores.txt para escribir.");
                                out << cedula << "," << claveCod << "," << nAdmin << "," << metodoAdmin << "\n";
                                out.close();

                                cout << "Administrador registrado exitosamente con n=" << nAdmin
                                     << " y metodo=" << metodoAdmin << ".\n";
                            }
                        } catch (const exception &e) {
                            cerr << "Error en modo SUPER ADMIN: " << e.what() << "\n";
                        }
                    }


                    // MODO ADMINISTRADOR

                    else if (modo == 2) {
                        try {
                            char cedulaAdmin[50], clave[50];
                            cout << "Ingrese cedula de admin: ";
                            cin.getline(cedulaAdmin, 50);
                            cout << "Ingrese clave: ";
                            cin.getline(clave, 50);
                            if (strlen(cedulaAdmin) == 0 || strlen(clave) == 0)
                                throw runtime_error("Debe ingresar cedula y clave.");

                            ifstream in("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/administradores.txt");
                            if (!in) throw runtime_error("No se encontro administradores.txt");

                            bool valido = false;
                            int nAdmin = 0, metodoAdmin = 0;
                            string linea;
                            while (getline(in, linea)) {
                                stringstream ss(linea);
                                string cedulaF, claveF, nF, metodoF;
                                getline(ss, cedulaF, ',');
                                getline(ss, claveF, ',');
                                getline(ss, nF, ',');
                                getline(ss, metodoF, ',');

                                if (cedulaF == cedulaAdmin) {
                                    nAdmin = stoi(nF);
                                    metodoAdmin = stoi(metodoF);

                                    string bits;
                                    for (unsigned char c : claveF)
                                        convertirByteABits_aplicacion(c, bits);

                                    string claveDec = (metodoAdmin == 1)
                                                          ? decodificarMetodo1_aplicacion(bits, nAdmin)
                                                          : decodificarMetodo2_aplicacion(bits, nAdmin);

                                    string claveReal = convertirBitsABytes_aplicacion(claveDec);

                                    if (strcmp(claveReal.c_str(), clave) == 0)
                                        valido = true;
                                    break;
                                }
                            }
                            in.close();

                            if (!valido) throw runtime_error("Admin no autorizado o clave incorrecta.");

                            cout << "Acceso como ADMINISTRADOR.\n";
                            cout << "Desea registrar un nuevo usuario? (s/n): ";
                            char op; cin >> op; cin.ignore();

                            if (op == 's' || op == 'S') {
                                char cedula[50], claveU[50], saldo[20];
                                cout << "Ingrese cedula del usuario: ";
                                cin.getline(cedula, 50);
                                cout << "Ingrese clave: ";
                                cin.getline(claveU, 50);
                                cout << "Ingrese saldo inicial: ";
                                cin.getline(saldo, 20);

                                if (strlen(cedula) == 0 || strlen(claveU) == 0 || strlen(saldo) == 0)
                                    throw runtime_error("Campos vacios al registrar usuario.");

                                int nUser, metodoUser;
                                cout << "Ingrese semilla (n) para el usuario: ";
                                cin >> nUser; cin.ignore();
                                cout << "Ingrese metodo (1 o 2): ";
                                cin >> metodoUser; cin.ignore();

                                if (nUser < 1 || nUser > 7) throw runtime_error("Semilla fuera de rango (1-7).");
                                if (metodoUser != 1 && metodoUser != 2) throw runtime_error("Método inválido (solo 1 o 2).");

                                string bits;
                                for (int i = 0; claveU[i] != '\0'; ++i)
                                    convertirByteABits_aplicacion((unsigned char)claveU[i], bits);

                                string cod = (metodoUser == 1)
                                                 ? codificarMetodo1_string(bits, nUser)
                                                 : codificarMetodo2_string(bits, nUser);

                                string claveCod = convertirBitsABytes_aplicacion(cod);

                                ofstream out("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/usuarios.txt", ios::app);
                                if (!out.is_open()) throw runtime_error("No se pudo abrir usuarios.txt para escribir.");
                                out << cedula << "," << claveCod << "," << saldo
                                    << "," << nUser << "," << metodoUser << "\n";
                                out.close();

                                cout << "Usuario registrado correctamente.\n";
                            }
                        } catch (const exception &e) {
                            cerr << "Error en modo ADMIN: " << e.what() << "\n";
                        }
                    }


                    // MODO USUARIO

                    else if (modo == 3) {
                        try {
                            char cedula[50], clave[50];
                            cout << "Ingrese cedula: ";
                            cin.getline(cedula, 50);
                            cout << "Ingrese clave: ";
                            cin.getline(clave, 50);
                            if (strlen(cedula) == 0 || strlen(clave) == 0)
                                throw runtime_error("Debe ingresar cedula y clave.");

                            ifstream in("C:/Users/SYSTICOM SOPORTE/Documents/UNIVERSIDAD/INFO 2/QT/Practica3/Practica3/usuarios.txt");
                            if (!in) throw runtime_error("No existe usuarios.txt");

                            bool encontrado = false;
                            string linea;
                            while (getline(in, linea)) {
                                stringstream ss(linea);
                                string cedulaF, claveF, saldoF, nF, metodoF;
                                getline(ss, cedulaF, ',');
                                getline(ss, claveF, ',');
                                getline(ss, saldoF, ',');
                                getline(ss, nF, ',');
                                getline(ss, metodoF, ',');

                                if (cedulaF == cedula) {
                                    int nUser = stoi(nF);
                                    int metodoUser = stoi(metodoF);

                                    string bits;
                                    for (unsigned char c : claveF)
                                        convertirByteABits_aplicacion(c, bits);

                                    string claveDec = (metodoUser == 1)
                                                          ? decodificarMetodo1_aplicacion(bits, nUser)
                                                          : decodificarMetodo2_aplicacion(bits, nUser);

                                    string claveReal = convertirBitsABytes_aplicacion(claveDec);

                                    if (strcmp(claveReal.c_str(), clave) == 0) {
                                        encontrado = true;
                                        int saldo = stoi(saldoF);
                                        int opcion;
                                        do {
                                            cout << "\n1. Consultar saldo\n2. Retirar dinero\n3. Salir\n> ";
                                            cin >> opcion;

                                            if (cin.fail()) {
                                                cin.clear();
                                                cin.ignore(1000, '\n');
                                                throw runtime_error("Entrada no valida. Ingrese un numero.");
                                            }

                                            if (opcion == 1) {
                                                saldo -= 1000;
                                                cout << "Saldo actual: " << saldo << "\n";
                                            } else if (opcion == 2) {
                                                int retiro;
                                                cout << "Monto a retirar: ";
                                                cin >> retiro;
                                                if (cin.fail() || retiro <= 0)
                                                    throw runtime_error("Monto invalido.");
                                                if (retiro + 1000 <= saldo) {
                                                    saldo -= retiro + 1000;
                                                    cout << "Retiro exitoso. Nuevo saldo: " << saldo << "\n";
                                                } else {
                                                    cout << "Fondos insuficientes.\n";
                                                }
                                            }
                                        } while (opcion != 3);
                                    }
                                }
                            }

                            in.close();
                            if (!encontrado) throw runtime_error("Usuario o clave incorrecta.");
                        } catch (const exception &e) {
                            cerr << "Error en modo USUARIO: " << e.what() << "\n";
                        }
                    }

                    else if (modo == 4) {
                        cout << "Regresando al menu principal...\n";
                        break;
                    } else {
                        cout << "Opción invalida.\n";
                    }

                    cout << "\nDesea volver al menu de modos? (s/n): ";
                    cin >> continuar;
                    cin.ignore();

                } while (continuar == 's' || continuar == 'S');

            } catch (const exception &e) {
                cerr << "Error general: " << e.what() << "\n";
            }





            break;
        }


        case 7:
            cout << "Saliendo del programa...\n";
            return 0;

        default:
            cout << "Opcion invalida. Intente de nuevo.\n";
            break;
        }

    }
    return 0;
}



