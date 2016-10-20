#include "Space/applikation.h"
#include "Attribute.h"
#include <list>
#include "Level.h"
#include <iostream>
#include <fstream>
#include "explosionen.h"
#include "Pickup.h"

#pragma once

using namespace std;

class spiel :public applikation
{
private:
	//______________________________________
	//Variablen f�r Dynamisches Objekteladen
	//
	//Anzahl der geladenen Objekte
	int				iAnzGelGegobj;
	int				iAnzGelLanobj;
	int				iAnzGelSchobj;
	int				iAnzGelPicobj;
	//List mit den geladenen Objekten
	list<objekt *>      lstGegnerobj;
	list<objekt *>      lstLandschaftobj;
	list<objekt *>		lstSchussobj;
	list<objekt *>		lstPickupobj;
	//Anzahl der geladenen Gegner/Schuss/Pickup Profile
	int				iProfGeg_count;
	int				iProfSch_count;
	int				iProfPic_count;
	int				iProfFor_count;
	//List mit geladenen Gegnertypen/Schuss/Pickup Profile
	list<Gegnerfilesystem *> lstGegnerfst;
	list<Schussfilesystem *> lstSchussfst;
	list<Pickupfilesystem *> lstPickupfst;
	list<Schussfilesystem *>::iterator iterSchussfst;
	list<Pickupfilesystem *>::iterator iterPickupfst;
	list<Formationfilesystem *> lstFormationfst;



	DIMOUSESTATE	maus;
	Spieler			*spieler;
	D3DXMATRIX		matrGlobalBewegung;
	D3DXMATRIX		matrMausbewegung;
	D3DXMATRIX		matrix;
	D3DXVECTOR3		vector;

	short int		spawn_timer;		// Timer zum Spawnen der Gegner
	float			fSpawntimeInSec;	// gibt die zeit zwischen Spawn in Sekunden an
	short int		iSteps;				// Schussfrequenz
	short int		iRot;				// Grad der Drehung beim hin-und herbewegen
	short int		iMenu;				// Auswahl eines Men�punktes
	short int		menu_beweg;			// Anzahl der noch zu erledigen Schritte bis zhum n�chsten Men�punkt
	short int		iFlagNameEingabe;
	short int		iFlagMenuSE;
	short int		iFlagVideo;
	short int		iFlagDoppelR;		//
	short int		iFlagDoppelL;		// Flags um das Doppeltdr�cken zu verhindern
	short int		iFlagRotR;			//
	short int		iFlagRotL;			// Flag um zu warten bis die Rotation vorbei is
	short int		iFlagL;				//
	short int		iFlagR;				// Flags, die das dauerhafte halten des knopfes verhindern
	short int		iFlagD;				//
	short int		iFlagU;				//
	short int		iFlagHS;			//
	short int		iFlagSET;
	short int		iFlagSTART;
	short int		iStandort;			// von welchem Men�punkt komme ich
	short int		iStandorth;			// hilfe f�r das Rotieren des Men�s
	unsigned char	key[256];			// Tastaturabfrage
	char			cName[255];
	char			highsc[20][255];	// Highscore einlesen
	char			c_score[20];		// Zum speichern des eingelesenen HS
	int				i_punkte[11];		// Integerwert des eingelesenen HS Wertes
	int				hszaehler;			// Z�hlvariable zum Einlesen der Highscore
	int				ALandElem;			// Anzahl der Landschaftselemente
	int				SettingsLvL;		// Vertikale Men�punktauswahl
	float			MausGe;				// Um die Mausgeschwindigkeit zu regeln
	short int		iSchild;			// Delay f�r Schildanzeige
	short int		iDead;				// Gegnersounditerator
	short int		iAlarm;				// Flag Alarmsound schon abgespielt?
	int				iAufladen;			// Delay f�r Regeneration
	int				iBlink;				// Delay um Raumschiffblinken zu realisieren
	short int		iPoweronoff;		// Flag um Sound abzuspielen, wenn Schild wieder aufgeladen
	int				iHallo;				// Flag um Begr��ungssound abzuspielen
	int				iPunkte;			// Punktez�hler
	int				iPunkteanzeige;		// Anzeige der Punkte
	int				iadd_points;
	int				iTime;				// Zeit die das Spiel schon l�uft
	int				iExplo;				// Dauer der Explosion
	int				iScheinwerfer;		// Dauer des Lichtes bei Explosion
	int				iScheinwerfer2;		// ?? Treffer von Spieler??
	int				iTada;				// D�d�d����d��d��d�����d��d��d�d����d�d���d�d����� Flag
	int				iGas;				// I think we may have a gas leak... Flag			
	int				iNebel;
	int				iNebelplus;
	int				iPickupzeit;		// Zeit f�r pickupspawn
	int				br,ho;
	short int		iPause;
	short int		iKeyPause;
	short int		iVideo;
	short int		iVidflag;
	

	skybox sky;
	video			videostart,video1,video2,video3,endlos;
	video			endlosvideo;
	
	D3DXVECTOR3 abc; /// ##### WOF�R BRAUCHEN WIR DAS?? SINNVOLLER NAME!!!!#######
	D3DXVECTOR3 vSpieleraltepos;

	objekt			schild;		    // Schildblase um das Raumschiff des Spielers
	objekt			*objLoeschbox;		// Begrenzung, in der Gegner/Sch�sse/etc. gerendert werden sollen
	objekt			*objMenu;			// Kugeln im Men�
	objekt			*objGameOver;
	objekt			*objGewonnen;
	Level			*level;				// das geladene Level (enth�lt Spawnpunkte + Gegnertypen)
	Pickup			*pPick;				// Pickuppointer

	list<Gegner *>	lstGegner;			// Liste der Gegner
	list<Schuss *>	lstSchuss;			// .. der Sch�sse
	list<Landschaft *>lstLandschaft;	// .. der Landschaften
	list<Schuss *>  lstGegSchuss;		// .. der Gegnersch�sse

	list<Landschaft *>::iterator iterLandschaft;
	list<Schuss *>::iterator iterSchuss;
	list<Gegner *>::iterator iterGegner;
	list<Schuss *>::iterator iterGegSchuss;

	int x,y,breite,hoehe;				// Ma�e des Spielfensters

	sprite HUD;							// HUD Sprite
	sprite Energiebalken[11];				// Energiebalken HUD
	sprite Finger;
	sprite Pause;

	mein_partikelsystem partAntrieb1;	// Partikelsysteme
	mein_partikelsystem partAntrieb2;
	mein_partikelsystem partExplosion;

	void kollisionGegnerschuesseSpieler();
	void kollisionSpielerschuesseGegner();
	void kollisionSpielerGegner();
	int  kollisionSpielerGegnerauth(D3DXVECTOR3 &vErste,D3DXVECTOR3 &vDritte,D3DXVECTOR3 &vsErste);
	void kollisionSpielerPickup();
	void createPickup();
	void initialisiereSprites();
	void get_screenresolution(int *x, int *y);
	void return_to_menu();

public:

	// -cDir_Name ist der Unterordner
	// -objektliste in der gespeichert werden soll
	// -fMaxwidth gibt an ob und ab welcher gr��e Skaliert werden soll (0=keine Skalierung)
	// NOTIZ: Ini files hei�en standardm��ig Modelle.ini
	int modelle_laden(char *cDir_name,list<objekt *> *lstType,float fMaxwidth=0);
	// Unterfunktion profmodload() regelt den Rest
	// hier nur in eine Funktion gepackt,
	// zur �bersicht beim Zusammenf�gen
	void profmodloadup();
	//______________________________________
	D3DXMATRIX get_globalBewegung() {return matrGlobalBewegung;}
	void verarbeite_eingaben();
	void gegner_spawnen();
	void setup();
	int render();
	void rendergame();
	void rendermenu();
	void renderhighs();
	void rendersettings();
	void rendergameover();
	void renderlevelcompleted();
	void spieleinst();
	void menueinst();
	void gameovereinst();
	void behandle_partikel();
	void AuswahlSpielStarten();
	void AuswahlHighscoreStarten();
	void AuswahlSettingsStarten();
	void rechts();
	void links();
	void MenuRotR();
	void MenuRotL();
	void Ueberpruefe();
	void Highscoreladen();
	void EingabeName();

	
};