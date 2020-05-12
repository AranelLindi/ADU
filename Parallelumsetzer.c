#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define line "############################################\n"

//###########################################################
// Eingangswerte:
float U_ref = 0;				// Referenzspannung
int bits = -1;					// Auflösung in Bits (aus taktischen Gründen -1 gesetzt!)
float U_in = 0;					// Eingangsspannung

// Zu berechnende Werte:
float U_LSB = 0;				// least significant bits
int Komps = 0;					// Anzahl Komperatoren
int Resistors = 0;				// Anzahl der Widerstände (natürlich Zahl!)
int steps = 0;					// 2^N Schritte

// Hilfsvariablen:
int Counter = 0;				// Zählt Wahr-Vergleich der Komperatoren
//###########################################################

int Komparator(float P, float N)	// Vergleicht Spannungen von P/N-Eingang
{
	return (P > N);
}

int main(void)
{
	// ****************************
	// 1. Benutzereingaben abfragen:
	printf("\n\t*** ADU-PARALLELUMSETZER ***\n\n");
	printf(line);
	printf("\t\tEINGABEN\n");

	while (U_ref <= 0) {
		printf(" Referenzspannung :");
		scanf("%f", &U_ref);
	}

	while (bits <= 0) {
		printf(" Anzahl Bits :");
		scanf("%i", &bits);
	}

	while (U_in <= 0) {
		printf(" Eingangsspannung :");
		scanf("%f", &U_in);
	}
	// ****************************

	// ****************************
	// 2. Weitere Werte berechnen:
	steps = (1 << bits);
	U_LSB = U_ref / steps;
	Komps = (steps - 1);
	Resistors = (steps - 2);

	printf(line);
	printf("\t\tDATEN\n");
	printf(" U_ref: %f V\n", U_ref);
	printf(" U_LSB: %f V\n", U_LSB);
	printf(" U_in: %f V\n", U_in);
	printf(" Bits: %i \n", bits);
	printf(" Schritte: %i \n", steps);
	printf(" Widerstaende: %i*R + 3/2*R + 1/2*R = %i*R\n", Resistors, steps);
	printf(" Komperatoren: %i \n", Komps);
	printf(line);
	// ****************************

	// ****************************
	// 3. Mittels Iteration Komparatorvergleiche durchführen und ausgeben:
	printf("\t\tBERECHNUNGEN\n");
	printf(" Komp-Nr.\tU_Vergleich\tSignal\n");

	for (int i = 0; i < Resistors; i++) {
		double angelegteSpannung = U_LSB * ((13.0 / 2) - (i * (6.0 / (steps - 2))));	// Jeweils anliegende Spannung am Komparator
		char Vergleich = Komparator(U_in, angelegteSpannung);	// 'true' oder 'false' Komp.-Vergleich
		if (Vergleich == 1) {
			Counter++;
		}
		printf(" %i \t\t%f V \t%i\n", Komps - i, angelegteSpannung, Vergleich);
	}

	// Letzen Komparatorvergleich seperat mit R/2 und U_LSB/2 berechnen und ausgeben:
	char Vergleich = Komparator(U_in, (U_LSB / 2));
	if (Vergleich == 1) {
		Counter++;
	}
	printf(" %i \t\t%f V \t%i\n", 1, U_LSB / 2, Vergleich);
	// ****************************

	// ****************************
	// 4. Ausgabe des Ergebnisses:
	int Z_ = Counter;			//Konvertieren(Counter);
	float U_dig = 1.0 * Z_ * U_LSB;	// 1.0 damit Compiler Fließkommazahl ausgibt

	printf(line);
	printf("\t\tERGEBNISSE\n");
	printf(" Digitalzahl Z: %i\n U_dig: %f V\n |Fehler|: %f V\n", Z_, U_dig, fabs(U_in - U_dig));
	printf(line);
	// ****************************

	return 0;
}
