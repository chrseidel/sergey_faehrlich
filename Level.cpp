#include "windows.h"
#include "Level.h"
#include <cstdlib>

using namespace std;

Level::Level(int Level)
{
	load_level(Level);
}

Level::~Level()
{
	while(!lstSpawnpunkt.empty() )
	{
		lstSpawnpunkt.pop_back();
	}
}

int Level::load_level(int iLevel)
{
	iSizeOfList = 0;
	spawn* new_gegner;
	

	char cCount[5];
	char cGegnertyp[7] = {"typ"}; // "typXXX0" = 6 Zeichen
	char cFormation[13] = {"formation"}; // "formationXXX0" = 12 Zeichen
	char cPosition[12] = {"position"};	// "positionXXX0" = 11 Zeichen

	char cLevel [4];
	itoa(iLevel,cLevel,4);

	char cDateipfad[20] = "Level\\level.ini";

	for (int i = 0; ;i++)
	{
		new_gegner = (spawn*)malloc(sizeof(spawn)); // Speicher für neuen Gegner reservieren
		itoa(i,cCount,4);							// Zählinteger in char wandeln um ihn hinten an die zeichenkette anzuhängen
		strcat_s(cGegnertyp,cCount);				// 
		strcat_s(cFormation,cCount);				// Strings zum Laden der Attribute erstellen
		strcat_s(cPosition,cCount);					//

		new_gegner->iGegnertyp = GetPrivateProfileInt(cLevel, cGegnertyp,-1,cDateipfad);
		new_gegner->iFormation = GetPrivateProfileInt(cLevel, cFormation,-1,cDateipfad); // Attribute in das neue Gegnerobjekt laden
		new_gegner->iPosition = GetPrivateProfileInt(cLevel, cPosition,-1,cDateipfad);

		cGegnertyp[3] = 0;
		cFormation[9] = 0; // String wieder vom Zählinteger befreien
		cPosition[8] = 0;

		// Wenn kein Gegner mehr geladen wurde abbrechen
		if (new_gegner->iFormation == -1 && new_gegner->iPosition == -1 && new_gegner->iGegnertyp == -1)
		{
			free(new_gegner);
			break;		// letzter (leere Element) löschen und Schleife zum Laden beenden
		}
		else
		{
			lstSpawnpunkt.push_back(new_gegner);
			iSizeOfList++;							// Gegnerobjekt an die Liste anhängen
		}
	}
	
	current_enemy = lstSpawnpunkt.begin();
	return iSizeOfList;
}

//zufällige Position in den Grenzen +/- Grenze
int Level::randomPosition(int Grenze)
{
	int value=((float)rand()/RAND_MAX)*(Grenze*2+1); // zufällige Zahl zwischen 0 und 2*Grenze
	value -= Grenze;
	if(value> (2*Grenze) || value < (-Grenze) ) // Wenn außerhalb des gewünschten Bereichs Wert auf 0 setzen
	{
		value = 0;
	}
	return value;
}

// Beschreibt die übergebenen Zeiger mit den Attributen des nächsten Gegners
void Level::get_next_enemy(int *Gegnertyp, int *Formation, int *Position)
{
	static int current_enemy_count = 1;
	*Gegnertyp = (*current_enemy)->iGegnertyp;
	*Formation = (*current_enemy)->iFormation;
	*Position = randomPosition((*current_enemy)->iPosition); // Randomzahl zwischen +iPosition und -iPosition;

	if (current_enemy_count == iSizeOfList)
	{
		current_enemy = lstSpawnpunkt.begin();
		current_enemy_count = 1;
	}
	else
	{
		current_enemy++;
		current_enemy_count++;
	}
}



