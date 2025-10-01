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





















#endif // DECLARACIONES_H
