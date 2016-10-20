#include "Schuss.h"
#include "Gegner.h"

Schuss::Schuss(objekt *copymodell, Schussfilesystem *sfstAttr,short int iGeg, int z)
{
	objekt::operator=(*copymodell);
	sfstAttribute=*sfstAttr;
	sfstAttribute.iGeschwindigkeitz=z;
	if(iGeg)
		sfstAttribute.iGeschwindigkeit*=-1;
}

void Schuss::step()
{

	// BEWEGEN
	D3DXMATRIX d3dxmOffset;
	D3DXMatrixTranslation(&d3dxmOffset,sfstAttribute.iGeschwindigkeit-abs(sfstAttribute.iGeschwindigkeitz),0.0f, ((float) sfstAttribute.iGeschwindigkeitz) /3);
	add_transform(&d3dxmOffset);
	render();
}

int Schuss::out(objekt *objLoeschbox)
{
	get_pos(vec3Position);
	if( !(objLoeschbox->inbox(&vec3Position)))
	{
		return 1;
	}
	return 0;
}