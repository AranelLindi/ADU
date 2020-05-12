#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define line "#####################################\n"

// Eingangswerte:
float U_ref = 0;				// Referenzspannung
int bits = 0;					// Anzahl Bits (Auflösung)
float U_in = 0;					// Eingangsspannung

// Zu berechnende Werte:
float U_LSB = 0;				// Spannung des niedrigstwertigsten Bits
int Z = 0;						// Die Digitalzahl Z
float U_dig = 0;				// digitalisierte Eingangsspannung (Z*U_LSB)

// Hilfsveriablen
int steps = 0;					// Anzahl der Vergleiche
char *values;					// 1 x bits-dimensionierter Vektor der gesetzte Bits speichert

char Komparator(float P, float N)	// Stellt den Vergleichskomparator dar
{
	return (P > N);				// Gibt 1 zurück falls P>N, ansonsten 0
}

int ZahlZ(void)					// gibt die bis dato bekannte Digitalzahl Z zurück
{
	int tempZ = 0;

	/* Bsp: 3 Bits: 4 2 1
	   _bit wird auf höchstwertigstes Bit (Bsp: 4) gesetzt.
	   Es wird durch values Vektor iteriert und alle
	   gesetzten Bits mit der entsprechenden Zahl
	   addiert.
	 */

	int _bit = (1 << (bits - 1));	// wird auf - der Auflösung entsprechend - höherwertigstes Bit gesetzt
	for (int i = bits; i > 0; i--) {
		if (values[bits - i]) {
			tempZ += _bit;
		}
		_bit = (_bit >> 1);		// wird auf nachfolgendes Bit verringert (z.B. von 4 auf 2)
	}

	return tempZ;
}

float U_v(void)					// Ausgabe der zu vergleichende Spannung U(Z)
{
	return ZahlZ() * U_LSB * 1.0;
}

int main(void)
{
	// ****************************
	// 1. Benutzereingaben abfragen:
	printf("\n\t*** ADU-sukzessive\n\t\t Approximation ***\n\n");
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

	printf(line);
	// ****************************

	// ****************************
	// 2. Weitere Werte berechnen:
	steps = (1 << bits);
	U_LSB = U_ref / steps;

	printf("\t\tWERTE\n");
	printf(" U_ref: %f V\n", U_ref);
	printf(" U_LSB: %f V\n", U_LSB);
	printf(" U_in: %f V\n", U_in);
	printf(" Bits: %i \n", bits);
	printf(line);

	values = (char *)malloc(bits * sizeof(char));
	// ****************************

	// ****************************
	// 3. Verfahren durchführen:
	printf("\t\tVERFAHREN\n");
	printf(" Schritt\tU(Z)\tBitbelegung\n");

	int Counter = 0;
	for (int i = 1; i < (bits + 1); i++) {
		values[i - 1] = 1;

		float cU_v = U_v();

		if (Komparator(U_in, cU_v) == 1) {
			// Fall: U_in > U_v

			Counter++;
		} else {
			// Fall: U_in < U_v
			values[i - 1] = 0;
		}
		printf(" %i\t\t%f V\t%i\n", i, cU_v, Komparator(U_in, cU_v));
	}

	printf(line);
	// ****************************

	// ****************************
	// 4. Ergebnisse:
	printf("\t\tERGEBNISSE\n");

	Z = ZahlZ();
	U_dig = Z * U_LSB * 1.0;

	printf(" Digitalzahl Z: %i\n", Z);
	printf(" U_dig: %f V\n", U_dig);
	printf(" |Fehler|: %f V\n", fabs(U_in - U_dig));
	printf(line);
	// ****************************

	free(values);				// Speicher freigeben!

	return 0;
}
