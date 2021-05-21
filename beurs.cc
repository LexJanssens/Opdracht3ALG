// Implementatie van klasse Beurs

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "beurs.h"

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

// zet een binaire string om naar een decimaal getal
int Beurs::binToDec(string bin) {
	int s = 0;
	int l = bin.size(); // lengte van de bitstring
	for (int i = 0; i < l; i++) {
		if (bin[i] == '1') {
			s += macht(2,l-i-1);
		}
	}
	return s;
}

//****************************************************************************

// zet een decimaal getal om naar een binaire string
string Beurs::decToBin(int dec) {
	string s;
	for (int i = n; i >= 0; i--) {
		if (dec - macht(2,i) >= 0) {
			dec -= macht(2,i);
			s.insert(0, "1");
		}
		else {
			s.insert(0, "0");
		}
	}
	return s;
}

//****************************************************************************

// returned de waarde van de aandelen op dag t
double Beurs::bepaalWaardeAandelen(int t, int aandelen)
{
	double waarde = 0;
	string s = decToBin(aandelen);

	for (int i = 0; i < n; i++) { // voor ieder aandeel
		if (s[i] == '1') { // als aandeel in bezit
			// voeg waarde aandeel toe
			waarde += dagen[t]->koersen[i];
		}
	}
	return waarde;
}

//****************************************************************************

// bepaald de nieuwe kas door de aandelen te kopen en te verkopens
double Beurs::bepaalKas(int t, double kas, int vorigeAandelen, int aandelen)
{
	bool vorigA, huidigA;
	string bin = decToBin(vorigeAandelen);
	string binNieuw = decToBin(aandelen);

	for (int i = 0; i < n; i++) { // voor ieder aandeel

		// kijkt of de huidige situatie aandeel i heeft
		if (bin[i] == '1') {
			vorigA = true;
		} else {
			vorigA = false;
		}

		// kijkt of de toekomstige situatie aandeel i heeft
		if (binNieuw[i] == '1') {
			huidigA = true;
		} else {
			huidigA = false;
		}

		// wanneer er een aandeel verkocht of gekocht word
		// worden de uitgaven/inkomsten - de provisie bij de nieuweKas gevoegd
		if (!(vorigA == huidigA)) {
			if (vorigA) {
				kas += dagen[t]->koersen[i]*(1-(provisie/100));
			} else {
				kas -= dagen[t]->koersen[i]*(1+(provisie/100));
			}
		}
	}
	return kas;
}
//****************************************************************************

// bepaald welke aandelen verkocht of gekocht moeten worden
vector <pair <bool,int>> Beurs::bepaalTransactie(int vorigeAandelen, int aandelen)
{
	vector <pair <bool,int>> transactie;
	bool vorigA, huidigA;
	string bin = decToBin(vorigeAandelen);
	string binNieuw = decToBin(aandelen);

	for (int i = 0; i < n; i++) { // voor ieder aandeel

		// kijkt of de huidige situatie aandeel i heeft
		if (bin[i] == '1') {
			vorigA = true;
		} else {
			vorigA = false;
		}

		// kijkt of de toekomstige situatie aandeel i heeft
		if (binNieuw[i] == '1') {
			huidigA = true;
		} else {
			huidigA = false;
		}
		if (!(vorigA == huidigA)) {
			if (vorigA) {
				transactie.push_back(make_pair(false, i));
			} else {
				transactie.push_back(make_pair(true, i));
			}
		}
	}
	return transactie;
}

//****************************************************************************

// 
double Beurs::bepaalMaxBedragBU
		(vector <vector <pair <bool,int>>> &transacties)
{
	int maxAandeel = macht(2,n);

	string bin;
	double kas;
	int prevAandeel;
	double maxKas = -1;
	
	// een 2d array met pairs waarvan de eerste de maximale
	pair <double, vector <vector <pair <bool,int>>>> bedrag[tw+1][maxAandeel];

	for (int i = 0; i < maxAandeel; i++) { 
		bedrag[0][i].first = b0-bepaalWaardeAandelen(0, i)*(1+(provisie/100));
		bedrag[0][i].second.push_back(bepaalTransactie(0,i));
	}

	for (int i = 1; i <= tw; i++) {
		for (int j = 0; j < maxAandeel; j++) {
			for (int k = 0; k < maxAandeel; k++) {
				kas = bedrag[i-1][k].first;
				if (kas >= 0) {
					kas *= 1+(dagen[i-1]->rente/100);
					kas = bepaalKas(i, kas, k, j);
				}
				if (kas > maxKas) {
					maxKas = kas;
					prevAandeel = k;
				}
			}
			bedrag[i][j].first = maxKas;
			maxKas = -1;
			bedrag[i][j].second = bedrag[i-1][prevAandeel].second;
			bedrag[i][j].second.push_back(bepaalTransactie(prevAandeel,j));
		}
	}
	transacties = bedrag[tw][0].second;
	return bedrag[tw][0].first;
}  // bepaalMaxBedragBU

//****************************************************************************

double Beurs::bepaalMaxBedragRecNoMemo(int t, double kas, int aandelen)
{
	double maxBedrag = 0;
	double bedrag;
	double nieuweKas;

	if (t == tw) {
		return kas + bepaalWaardeAandelen(t, aandelen)*(1.0-(provisie/100));
	}
	for (int i = 0; i < macht(2, n); i++) { // voor alle mogenlijke aandelen in bezig
		nieuweKas = bepaalKas(t, kas, aandelen, n);

		if (nieuweKas >= 0) {
			nieuweKas *= 1.0+(dagen[t]->rente/100); // krijgen van rente
			bedrag = bepaalMaxBedragRecNoMemo(t+1, nieuweKas, i);

			if (bedrag > maxBedrag) {
				maxBedrag = bedrag;
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
			(vector <vector <pair <bool,int>>> transacties)
{
	int grote;
	for (int i = 0; i <= tw; i++) {
		cout << endl << "Dag: " << i;

		grote = transacties[i].size();

		cout << endl << "Verkoop aandelen: ";
		for (int j = 0; j < grote; j++) {
			if (!transacties[i][j].first) {
				cout << transacties[i][j].second << ", ";
			} 
		}

		cout << endl << "Koop aandelen: ";
		for (int j = 0; j < grote; j++) {
			if (transacties[i][j].first) {
				cout << transacties[i][j].second << ", ";
			} 
		}
		cout << endl;
	}
}  // drukAfTransacties

