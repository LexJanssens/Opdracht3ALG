// Implementatie van klasse Beurs

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "beurs.h"
#include <cmath>

using namespace std;

//*************************************************************************

// Default constructor
Beurs::Beurs ()
{
// TODO: implementeer (zo nodig) deze constructor

}  // default constructor

//****************************************************************************

// Lees de invoerparameters en -waardes in uit tekstbestand invoernaam.
bool Beurs::leesIn(const char* invoernaam)
{
	ifstream invoer (invoernaam, ios::in);

	if (!invoer.is_open()) { // Check of de file geopened kan worden
		cout << "Kan file niet openen." << endl;
		return false;
	}
	invoerNaam = invoernaam; // sla de naam op in de classe

	invoer >> tw;
	invoer >> n;

	// checkt en tw en n binnen de grenzen ligen
	if (!(1 <= tw && tw <= MaxTw && 1 <= n && n <= MaxN)) {
		cout << "Tw en/of n ligt/liggen niet binnen de grenzen." << endl;
		return false;
	}

	invoer.get();
	invoer >> provisie;
	invoer.get();
	invoer >> b0;

	// leest de koersen in
	for(int i = 0; i < tw+1; i++) {
		dagen[i] = new Dag;
		invoer.get();
		for(int j = 0; j < n; j++) {
			invoer >> dagen[i]->koersen[j];
		}
	}

	// leest de rentes in
	for (int i = 0; i < tw; i++) {
		invoer.get();
		invoer >> dagen[i]->rente;
	}

	return true;
}  // leesIn

//****************************************************************************

void Beurs::drukAfInvoer()
{
	cout << "--- " << invoerNaam << " ---" << endl;
	cout << "Bedrag wereldreis: " << tw << endl;
	cout << "Dagen tot wereldreis: " << tw+1 << endl;
	cout << "Provisie: " << provisie << endl;
	cout << "Beginbedrag: " << b0 << endl;
	cout << "Bedrijven: "  << n << endl;
	for (int i = 0; i < tw+1; i++) {
		cout << "  Dag " << i << ": " << endl;
		cout << "    Koersen: ";
		for (int j = 0; j < n; j++) {
			cout << dagen[i]->koersen[j] << ", ";
		}
		cout << endl;
		if (i != tw){
			cout << "    Rente: " << dagen[i]->rente << endl;
		}
	}
	cout << "--- " << invoerNaam << " ---" << endl;
}  // drukAfInvoer

//****************************************************************************

// returned de waarde wanneer alle aandelen verkocht zouden worden op dag t
double Beurs::bepaalWaarde(int t, double kas, int aandelen)
{
	int power;

	for (int i = n-1; i >= 0; i--) { // voor ieder aandeel
		power = (int)pow(2, i);
		if (aandelen / power >= 1) { // als aandeel in bezit
			aandelen -= power;
			// voeg waarde aandeel toe aan de kas
			kas += dagen[t]->koersen[i]*(1.0-(provisie/100));
		}
	}
	return kas;
}

//****************************************************************************

double Beurs::bepaalKas(int t, double kas, int aandelen, int nieuweAandelen)
{
	int power;
	bool huidigA, nieuwA;

	for (int i = n-1; i >= 0; i--) { // voor ieder aandeel
		power = (int)pow(2, i);

		// kijkt of de huidige situatie aandeel i heeft
		huidigA = false;
		if (aandelen / power >= 1) {
			aandelen -= power;
			huidigA = true;
		}

		// kijkt of de toekomstige situatie aandeel i heeft
		nieuwA = false;
		if (nieuweAandelen / power >= 1) {
			nieuweAandelen -= power;
			nieuwA = true;
		}

		// wanneer er een aandeel verkocht of gekocht word
		// worden de uitgaven/inkomsten - de provisie bij de nieuweKas gevoegd 
		if (!(huidigA == nieuwA)) {
			if (huidigA) {
				kas += dagen[t]->koersen[i]*(1-(provisie/100));
			} else {
				kas -= dagen[t]->koersen[i]*(1+(provisie/100));
			}
		}
	}
	return kas;
}
//****************************************************************************

double Beurs::bepaalMaxBedragBU
		(vector <vector <pair <bool,int>>> &transacties)
{
	// TODO: implementeer deze memberfunctie

	return 0.0;
}  // bepaalMaxBedragBU

//****************************************************************************

double Beurs::bepaalMaxBedragRecNoMemo(int t, double kas, int aandelen)
{
	double maxBedrag = 0;
	double bedrag;
	double nieuweKas;

	if (t == tw) {
		return bepaalWaarde(t, kas, aandelen);
	}
	for (int i = 0; i < (int)pow(2, n); i++) {
		double oud = bepaalWaarde(t, kas, aandelen);
		double nieuw = bepaalWaarde(t, nieuweKas, i);
		cout << "oud: " << oud << endl;
		cout << "nieuw: " << nieuw << endl;

		if (nieuweKas >= 0 &&  nieuw >= oud) {

			nieuweKas *= 1.0+(dagen[t]->rente/100); // krijgen van rente
			bedrag = bepaalMaxBedragRecNoMemo(t+1, nieuweKas, i);

			if (bedrag > maxBedrag) {
				cout << bedrag << endl;
				maxBedrag = bedrag;
			}
		}
	}

	return maxBedrag;
}  // bepaalMaxBedragRec (memo)

double Beurs::bepaalMaxBedragRecNoMemo2(int t, double kas, int aandelen)
{
	double maxBedrag = 0;
	double bedrag;
	double nieuweKas;

	if (t == tw) {
		return bepaalWaarde(t, kas, aandelen);
	} else {
		return
		for (int i = 0; i < (int)pow(2, n); i++) {
			double oud = bepaalWaarde(t, kas, aandelen);
			double nieuw = bepaalWaarde(t, nieuweKas, i);
			cout << "oud: " << oud << endl;
			cout << "nieuw: " << nieuw << endl;

			if (nieuweKas >= 0 &&  nieuw >= oud) {

				nieuweKas *= 1.0+(dagen[t]->rente/100); // krijgen van rente
				bedrag = bepaalMaxBedragRecNoMemo(t+1, nieuweKas, i);

				if (bedrag > maxBedrag) {
					cout << bedrag << endl;
					maxBedrag = bedrag;
				}
			}
		}
	}
	return maxBedrag;
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

double Beurs::bepaalMaxBedragRec(bool memo)
{
	return bepaalMaxBedragRecNoMemo(0, b0, 0);
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
			(vector <vector <pair <bool,int> > > transacties)
{
// TODO: implementeer deze memberfunctie

}  // drukAfTransacties

