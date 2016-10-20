#include "SpaCE/applikation.h"
#include "Gegner.h"
#pragma once

class Schuss : public objekt
{
private:
	struct Schussfilesystem sfstAttribute;
	D3DXVECTOR3 vec3Position;

public:
	int get_staerke() {return sfstAttribute.iStaerke;}
	int out(objekt *objLoeschbox);
	Schuss(objekt *copymodell, Schussfilesystem *sfstAttr,short int iGeg=0,int z=0);
	void step();
	D3DXVECTOR3 get_Position() { return vec3Position; }
	float get_Schrittweite() {return sfstAttribute.iGeschwindigkeit;}
};