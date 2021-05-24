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
} // default constructor

//****************************************************************************

// Lees de invoerparameters en -waardes in uit tekstbestand invoernaam.
// True word gereturned als het lukt om het tekstbestand te openen, en de eerste
// twee getallen binnen de grenzen van de opdracht liggen
bool Beurs::leesIn(const char* invoernaam)
{
	ifstream invoer (invoernaam, ios::in);

	if (!invoer.is_open()) { // Check of de file geopened kan worden
		cout << "Kan file niet openen." << endl;
		return false;
	}
	invoerNaam = invoernaam; // sla de invoernaam op

	invoer >> tw;
	invoer >> n;

	// checkt of tw en n binnen de grenzen ligen
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
} // leesIn

//****************************************************************************

// Drukt, ter controle, de complete invoer af op het scherm.
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

} // drukAfInvoer

//****************************************************************************

// Zet een binaire string om naar een decimaal getal en returned dit getal
int Beurs::binToDec(string bin) {
	int s = 0;
	int l = bin.size(); // lengte van de bitstring
	for (int i = 0; i < l; i++) {
		if (bin[i] == '1') {
			s += macht(2,l-i-1);
		}
	}
	return s;
} // binToDec

//****************************************************************************

// Zet een decimaal getal om naar een binaire string en returned deze string
string Beurs::decToBin(int dec) {
	string s;
	for (int i = n; i >= 0; i--) {
		if (dec-macht(2,i) >= 0) {
			dec -= macht(2,i);
			s.insert(0, "1");
		}
		else {
			s.insert(0, "0");
		}
	}
	return s;
} // decToBin

//****************************************************************************

// returned de waarde van de aandelen op dag t
double Beurs::bepaalWaardeAandelen(int t, int aandelen)
{
	double waarde = 0;
	string bin = decToBin(aandelen);

	for (int i = 0; i < n; i++) { // voor ieder aandeel
		if (bin[i] == '1') { // als aandeel in bezit
			waarde += dagen[t]->koersen[i]; // voeg waarde aandeel toe
		}
	}
	return waarde;
} // bepaalWaardeAandelen

//****************************************************************************

// bepaald de nieuwe kas door de juiste aandelen te kopen en te verkopen
double Beurs::bepaalKas(int t, double kas, int vorigeAandelen, int aandelen)
{
	string bin = decToBin(vorigeAandelen); 
	string binNieuw = decToBin(aandelen);

	for (int i = 0; i < n; i++) { // voor ieder aandeel

		// wanneer er een aandeel verkocht of gekocht word
		// worden de uitgaven/inkomsten - de provisie bij de nieuweKas gevoegd
		if ((bin[i] == '1') != (binNieuw[i] == '1')) { // er word gekocht/verkocht
			if (bin[i] == '1') { // als aandeel i verkocht word
				kas += dagen[t]->koersen[i]*(1-(provisie/100));
			} else { // als aandeel i gekocht word
				kas -= dagen[t]->koersen[i]*(1+(provisie/100));
			}
		}
	}
	return kas;
} // bepaalKas
//****************************************************************************

// Bepaald welke aandelen verkocht of gekocht moeten worden
// Returned een vector van pairs, de boolean is true als aandeel i gekocht
// moet worden, en false als aandeel i verkocht moet worden.
vector <pair <bool,int>> Beurs::bepaalTransactie(int vorigeAandelen, int aandelen)
{
	vector <pair <bool,int>> transactie;
	string bin = decToBin(vorigeAandelen);
	string binNieuw = decToBin(aandelen);

	for (int i = 0; i < n; i++) { // voor ieder aandeel
		if ((bin[i] == '1') != (binNieuw[i] == '1')) { // als er een verandering is
			if (bin[i] == '1') { // als aandeel i verkocht word
				transactie.push_back(make_pair(false, i));
			} else { // als aandeel i gekocht word
				transactie.push_back(make_pair(true, i));
			}
		}
	}
	return transactie;
} // bepaalTransactie

//****************************************************************************

// Bepaald met behulp van bottom-up dynamisch programmeren het maximale
// bedrag dat de belegger aan het eind van dag tw in kas kan hebben
// (op dat moment heeft hij geen aandelen meer).
// In de parameter transacties komen de transacties die nodig zijn om tot
// het maximale bedrag te komen.
double Beurs::bepaalMaxBedragBU(vector <vector <pair <bool,int>>> &transacties)
{
	int prevAandeel;
	int maxAandeel = macht(2,n);
	double kas;
	double maxKas = -1;
	
	// een 2d array met pairs waarvan de eerste de maximale kas is en de tweede 
	// de transacties die nodig zijn om tot dit maximale bedrag te komen.
	pair <double, vector <vector <pair <bool,int>>>> bedrag[tw+1][maxAandeel];

	// de eerste rij van de tabel word ingevuld
	for (int i = 0; i < maxAandeel; i++) {
		bedrag[0][i].first = b0-bepaalWaardeAandelen(0, i)*(1+(provisie/100));
		bedrag[0][i].second.push_back(bepaalTransactie(0,i));
	}

	// de rest van de tabel word ingevuld
	for (int i = 1; i <= tw; i++) {
		for (int j = 0; j < maxAandeel; j++) {
			// voor elk mogenlijk aandelen bezig op tijd= i-1 word gekeken
			// welke op tijd=i de grootste kas zal opleveren
			for (int k = 0; k < maxAandeel; k++) {
				kas = bedrag[i-1][k].first;
				if (kas >= 0) { // kas mag niet negatief zijn
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
} // bepaalMaxBedragBU

//**************************************************************************** 

// Bepaald recursief/top-down het maximale bedrag in de kas op tijd=t met aandelen=a
// Deze worden opgeslagen in bedrag[][MaxAs] en gereturned
double Beurs::bepaalMaxBedragRecMemo(int t, int a, double bedrag[][MaxAs])
{
	double score;
	double maxScore = -2;
	if (bedrag[t][a] != -1) { // als het bedrag al bekent is
		return bedrag[t][a];
	} else {
		if (t == 0) { // als we bij de eerste rij zijn
			bedrag[0][a] = b0-bepaalWaardeAandelen(0, a)*(1+(provisie/100));
		} else {
			for (int i = 0; i < macht(2, n); i++) {
				score = bepaalMaxBedragRecMemo(t-1, i, bedrag);
				if (score >= 0) { // kas mag niet negatief zijn
					score *= 1+(dagen[t-1]->rente/100);
					score = bepaalKas(t, score, i, a);
					if (score > maxScore) {
						maxScore = score;
					}
				}
			}
			bedrag[t][a] = maxScore;
			maxScore = -2;
		}
		return bedrag[t][a];
	}
}  // bepaalMaxBedragRecMemo (memo)

//****************************************************************************

// Recursieve aanroep die het maximale bedrag in de kas op tw returned
// beginnend op tijd=t met kas=kas en aandelen=aandelen 
// Er worden geen deelresultaten opgeslagen
double Beurs::bepaalMaxBedragRecNoMemo(int t, double kas, int aandelen)
{
	double bedrag, nieuweKas;
	double maxBedrag = -1;

	if (t == tw) {
		return kas + bepaalWaardeAandelen(t, aandelen)*(1.0-(provisie/100));
	} else {
		for (int i = 0; i < macht(2, n); i++) {
			nieuweKas = bepaalKas(t, kas, aandelen, i);

			if (nieuweKas >= 0) { // kas mag niet negatief zijn
				nieuweKas *= 1+(dagen[t]->rente/100); // krijgen van rente
				bedrag = bepaalMaxBedragRecNoMemo(t+1, nieuweKas, i);

				if (bedrag > maxBedrag) {
					maxBedrag = bedrag;
				}
			}
		}
		return maxBedrag;
	}
}  // bepaalMaxBedragRecNoMemo (memo)

//****************************************************************************

// Bepaal recursief het maximale bedrag dat de belegger aan het eind van dag tw 
// in kas kan hebben (op dat moment heeft hij geen aandelen meer). Als parameter 
// memo = true worden berekende deelresultaten opgeslagen in tabel bedrag en 
// hergebruikt. Als memo = false, worden deelresultaten steeds opnieuw berekend.
double Beurs::bepaalMaxBedragRec(bool memo)
{

	if (memo) {
		double bedrag[tw+1][MaxAs];

		// lijst initialiseren met -1;
		for (int i = 0; i <= tw; i++) {
			for (int j = 0; j < macht(2,n); j++) {
				bedrag[i][j] = -1;
			}
		}
		return bepaalMaxBedragRecMemo(tw, 0, bedrag);
	} else {
		return bepaalMaxBedragRecNoMemo(0, 20, 0);
	}
}  // bepaalMaxBedragRec (memo)

//****************************************************************************

// De transacties die nodig zijn om tot het max bedrag te komen worden afgedrukt.
// Berekend ook, uitgaande van startbedrag b0, het resulterende eindbedrag, 
// en drukt dat af op het scherm.
void Beurs::drukAfTransacties(vector <vector <pair <bool,int>>> transacties)
{
	int grote;
	int aandeel;
	double kas = b0;

	for (int i = 0; i <= tw; i++) {
		grote = transacties[i].size();

		cout << endl << "Dag: " << i;

		cout << endl << "    Verkoop aandelen: ";
		// alle te verkopen aandelen worden afgedrukt en verkocht
		for (int j = 0; j < grote; j++) {
			if (!transacties[i][j].first) {
				aandeel = transacties[i][j].second
				cout << aandeel << ", ";
				kas += dagen[i]->koersen[aandeel]*(1-(provisie/100));
			} 
		}

		cout << endl << "    Koop aandelen: ";
		// alle te kopen aandelen worden afgedrukt en gekocht
		for (int j = 0; j < grote; j++) {
			if (transacties[i][j].first) {
				aandeel = transacties[i][j].second
				cout << aandeel << ", ";
				kas -= dagen[i]->koersen[aandeel]*(1+(provisie/100));
			} 
		}
		// rente word berekent
		if (i != tw) { // als niet de laastse dag
			kas *= 1+(dagen[i]->rente/100);
		}
	}
	cout << endl << endl << "MaxBedrag = " << kas << endl;
}  // drukAfTransacties

