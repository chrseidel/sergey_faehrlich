#include "Spieler.h"

Spieler::Spieler()
{
	iHp = 100;
	iWaffeAusgewaehlt=0;
	iWaffentimer = 0;
	load("Raumschiff.x","Raumschiff",D3DCULL_NONE);
	
	////////////////////////////////////
	D3DXMATRIX help;
	D3DXMatrixRotationY(&help,180*D3DX_PI/180);
	add_transform(&help);
	/////////////////////////////////////
}

//bei 0 hp return 1;
int Spieler::treffer(int iStaerke)
{
 iHp-=iStaerke;
 if (iHp<=0)
	 return 1;

 return 0;
}

void Spieler::hp_aufladen(int iWert)
{
  if(iHp+iWert >= 100)
	  iHp=100;
  else
	  iHp+=iWert;
}