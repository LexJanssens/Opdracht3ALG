// Implementatie van standaard functies.
// standaard.cc
// Geschreven door : Bart Aaldering (s2969866) en Lex Janssens (s2989344)
// Compiler: GNU GCC Compiler
// Beschrijving:
/*
	Hier staan enkele hulpfuncties die gebruikt worden in het 
	hoofdprogramma. Dat
*/
#include <iostream>
#include <iomanip>  // voor aantal decimalen double in uitvoer
#include <cstdlib>  // voor rand
#include "standaard.h"
using namespace std;

//*************************************************************************

//Geeft true terug wanneer waarde tussen minWaarde en maxWaarde ligt
bool integerInBereik (const char *variabele, int waarde,
					int minWaarde, int maxWaarde)
{
	if (waarde>=minWaarde && waarde<=maxWaarde)
		return true;
	else
	{ cout << variabele << "=" << waarde << ", maar moet in [" << minWaarde
			<< "," << maxWaarde << "] liggen." << endl;
		return false;
	}
}  // integerInBereik

//*************************************************************************

//Genereert een random getal tussen min en max
int randomGetal (int min, int max)
{
	int bereik,
	r;

	bereik = max - min + 1;

	r = ((rand())%bereik) + min;
	return r;
}  // randomGetal

//****************************************************************************

//Genereert een random double tussen min en max
double randomDouble (double min, double max)
{
	int bereik, r;
	double fr;

	bereik = (int)(100*(max - min) + 1);

	r = (rand())%bereik;  // 0 <= r <= 100*(max-min)
	fr = min + 0.01*r;
	return fr;
}  // randomDouble

//****************************************************************************

//Print een double met precisie
void schrijfDouble (double getal, int precisie, bool metEndln)
{
	cout << setiosflags (ios::fixed) << setprecision (precisie) << getal;
	if (metEndln)
		cout << endl;
}  // schrijfDouble

//****************************************************************************

//Berekent de macht van een grondgetal tot de macht exponent.
int macht(int grondtal, int exponent)
{
	int macht = 1;
	for (int i = 0; i < exponent; i++) {
		macht *= grondtal;
	}
	return macht;
} // macht
