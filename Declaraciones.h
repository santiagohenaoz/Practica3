#include <fstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifndef DECLARACIONES_H
#define DECLARACIONES_H



//------------ FUNCIONES DE CODIFICACION CON CHAR------------------------------

void byteToBits(unsigned char byte, char *bits);

void encodeMethod1_fixed(const char *origBits, char *outBits, int totalBits, int n);

void encodeMethod2_fixed(const char *origBits, char *outBits, int totalBits, int n);



//------------ FUNCIONES DE CODIFICACION CON STRING------------------------------

void byteToBits(unsigned char byte, string &bits);

string encodeMethod1_fixed(const string &bits, int n);

string encodeMethod2(const string &bits, int n);




//----- FUNCIONES DE  DECODIFICADOR CON CHAR ---------------------------------------


void byteToBits_dec(unsigned char byte, char *bitArray);

unsigned char bitsToByte_dec(const char *bitArray);

void metodo1_decodificar(const char *entradaBits, char *salidaBits, int totalBits, int n);

void metodo2_decodificar(const char *entradaBits, char *salidaBits, int totalBits, int n);



//----- FUNCIONES DE  DECODIFICADOR CON STRING ---------------------------------------
void byteToBits_dec(unsigned char byte, string &bits);

unsigned char bitsToByte_dec(const string &bits, int start);

string decodeMethod1(const string &bits, int n);

string decodeMethod2(const string &bits, int n);

string convertirBitsABytes_string(const string &bits);






//---------------FUNCIONES CLONADAS-----------------------------------------------------


void convertirByteABits_aplicacion(unsigned char byte, string &bits);

string convertirBitsABytes_aplicacion(const string &bits);

string codificarMetodo1_string(const string &bits, int n);

string decodificarMetodo1_aplicacion(const string &bits, int n);

string codificarMetodo2_string(const string &bits, int n);

string decodificarMetodo2_aplicacion(const string &bits, int n);

string leerArchivoBinario(const string &ruta);

void guardarArchivoBinario(const string &ruta, const string &data);















#endif // DECLARACIONES_H
