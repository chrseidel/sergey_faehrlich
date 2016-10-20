#include "SpaCE/applikation.h"
#include "Attribute.h"
#pragma once

class Gegner : public objekt
{
private: 
	struct Gegnerfilesystem gfstAttribute;
	D3DXMATRIX matrStepTranslation;
	D3DXVECTOR3 vec3Position;

	int iEnergie;

public:
	Gegner(objekt *copymodell, Gegnerfilesystem *gfstAttr, D3DXMATRIX &matrGegnerSpawn);
	short int treffer(int iSchaden);
	short int step();
	int out(objekt *objLoeschbox);
	int get_waffe(){return gfstAttribute.iWaffe;}
	int get_Schusstyp(){return gfstAttribute.iSchusstyp;}
	D3DXVECTOR3 get_position() {return vec3Position;}		
};