# include "SpaCE/applikation.h"
#pragma once

/*

Landschaften werden bei beginn des Spiels alle geladen und nicht gelöscht
Werden nur hinten dran geklont

*/

class Landschaft : public objekt
{
private:

public:
	Landschaft(objekt *copymodell, D3DXMATRIX &d3dxmOffset);
	void step();
	~Landschaft();
};