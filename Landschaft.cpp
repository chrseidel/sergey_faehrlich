# include "Landschaft.h"
#include "Liste.h"



Landschaft::Landschaft(objekt *copymodell, D3DXMATRIX &d3dxmOffset)
{	
	//Verschiedene Mapbausteine hier Laden

	objekt::operator=(*copymodell);
	//Offset
	add_transform(&d3dxmOffset);
}

void Landschaft::step()
{
}

Landschaft::~Landschaft()
{
}