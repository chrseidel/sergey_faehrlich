#include "Gegner.h"
#include "Schuss.h"
#include "Attribute.h"
#include "time.h"

#define GESCHWINDIGKEIT_TEST 3.0f



Gegner::Gegner(objekt* copymodell, Gegnerfilesystem* gfstAttr, D3DXMATRIX &matrGegnerSpawn)
{
	objekt::operator=(*copymodell);
	gfstAttribute=*gfstAttr;
		
	//Modell drehen
	// MUSS WIEDER RAUS NUR ZUM TESTEN WEIL DAS MODELL FALSCH IST --------
	D3DXMATRIX help;
	D3DXMatrixRotationY(&help,180*D3DX_PI/180);
	add_transform(&help);
	//---------------------------------------------------

	//Test
	srand(timeGetTime());
	gfstAttribute.iSchusstakt=rand()%(20)+15;
	gfstAttribute.iSchusstaktc=0;

	// Gegner 300 "Meter" vor dem Spieler spawnen
	D3DXMATRIX d3dxmOffset;
	vec3Position = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXMatrixTranslation(&d3dxmOffset,matrGegnerSpawn._41+300.0f,0.0f,0.0f);
	D3DXVec3TransformCoord(&vec3Position,&vec3Position,&d3dxmOffset);
	add_transform(&d3dxmOffset);
	//Gegner mit den Werten aus dem Attributesystem belegen
	int random=(rand()%(2))-1;
	if(random==0)
	gfstAttribute.iGeschwindigkeitz*=-1;

	D3DXMatrixTranslation(&matrStepTranslation,-gfstAttribute.iGeschwindigkeit,0.0f,gfstAttribute.iGeschwindigkeitz);

	iEnergie = gfstAttribute.iHp;
}

short int Gegner::step()
{
	D3DXVECTOR3 vec;
    get_pos(vec);
	if(vec.z>=22 || vec.z<=-22)
		matrStepTranslation._43=-matrStepTranslation._43;

	add_transform(&matrStepTranslation);
	//D3DXVec3TransformCoord(&vec3Position,&vec3Position,&matrStepTranslation);

	render();

	if(gfstAttribute.iSchusstaktc)
	{
		gfstAttribute.iSchusstaktc--;
		return 0;
	}
	else
	{
		gfstAttribute.iSchusstaktc=gfstAttribute.iSchusstakt;
		return 1;
	}
}

short int Gegner::treffer(int iSchaden)
{
	iEnergie-=iSchaden;
	if (iEnergie>0)
		return 0;

	return gfstAttribute.iPunkte;
}
int Gegner::out(objekt *objLoeschbox)
{
	get_pos(vec3Position);
	if( !(objLoeschbox->inbox(&vec3Position)))
	{
		return 1;
	}
	return 0;
}