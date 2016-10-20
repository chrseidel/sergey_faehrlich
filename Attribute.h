#pragma once
#define MAPBREITE 55
#define MAXVORNE 260
#define MAXHINTEN 80
#define GLOBALER_BEWEGUNGSVEKTOR 1.0f,0.0f,0.0f
#include <list>

using namespace std;

class partikeldaten
	{
	public:
		D3DXVECTOR3 speed;
		int life;
		int steps;
	};

//Die Attribute die Gegner haben sollen
struct Gegnerfilesystem
{
	int iHp;
	int iWaffe;
	int iGeschwindigkeit;
	int iGeschwindigkeitz;
	int iSchusstyp;
	int iModell;
	int iSchusstakt;
	int iSchusstaktc;
	int iPunkte;
	//usw
};
//Die Attribute die Schüsse haben sollen 
struct Schussfilesystem
{
	int iStaerke;
	int iGeschwindigkeit;
	int iGeschwindigkeitz;
	int iModell;
	//usw
};
//Die Attribute die Pickups haben sollen 
struct Pickupfilesystem
{
	int iWert;
	int iModell;
	//usw
};

struct partikelinf
{
	D3DXVECTOR3 speed;
	int TTL;
};

struct	Formationfilesystem
{
	list<D3DXVECTOR3 *> lstVecFormation;
	
};