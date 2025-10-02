#include <fstream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifndef DECLARACIONES_H
#define DECLARACIONES_H



//------------ EJERCICIOS DE DECODIFICACION CON CHAR------------------------------

void byteToBits(unsigned char byte, char *bits);

void encodeMethod1_fixed(const char *origBits, char *outBits, int totalBits, int n);

void encodeMethod2_fixed(const char *origBits, char *outBits, int totalBits, int n);



//------------ EJERCICIOS DE DECODIFICACION CON STRING------------------------------

void byteToBits(unsigned char byte, string &bits);

string encodeMethod1_fixed(const string &bits, int n);

string encodeMethod2(const string &bits, int n);




//----- PROGRAMA DECODIFICADOR CON CHAR ---------------------------------------


void byteToBits_dec(unsigned char byte, char *bitArray);

unsigned char bitsToByte_dec(const char *bitArray);

void metodo1_decodificar(const char *entradaBits, char *salidaBits, int totalBits, int n);

void metodo2_decodificar(const char *entradaBits, char *salidaBits, int totalBits, int n);



//----- PROGRAMA DECODIFICADOR CON STRING ---------------------------------------

void convertirByteABits_string(unsigned char byte, string &bits);


string decodificarMetodo1_string(const string &bits, int n);


string decodificarMetodo2_string(const string &bits, int n);

string convertirBitsABytes_string(const string &bits);



//---------------APLICACION-----------------------------------------------------------------------


string encodeMethod1_aplicacion(const string &bits, int n);

string encodeMethod2_aplicacion(const string &bits, int n);















#endif // DECLARACIONES_H
