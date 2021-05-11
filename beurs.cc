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


bool Beurs::leesIn (const char* invoernaam)
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

void Beurs::drukAfInvoer ()
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

// TODO: implementeer deze memberfunctie

}  // drukAfInvoer

//****************************************************************************

double Beurs::bepaalMaxBedragBU
		(vector <vector <pair <bool,int> > > &transacties)
{
// TODO: implementeer deze memberfunctie

return 0.0;

}  // bepaalMaxBedragBU

//****************************************************************************

double Beurs::bepaalMaxBedragRec (bool memo)
{
// TODO: implementeer deze memberfunctie

return 0.0;

}  // bepaalMaxBedragRec (memo)

//****************************************************************************

void Beurs::drukAfTransacties
			(vector <vector <pair <bool,int> > > transacties)
{
// TODO: implementeer deze memberfunctie

}  // drukAfTransacties

