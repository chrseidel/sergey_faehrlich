#include "SpaCE/applikation.h"
#pragma once
#define WAFFENTIMER_GROSSEWAFFE 200
#define WAFFENTIMER_KLEINEWAFFE 300



class Spieler : public objekt
{
private:
	int iHp;
	int iWaffeAusgewaehlt;
	int iWaffentimer;
	
public:
	Spieler();
	int  treffer(int iStaerke);
	void schild_aufladen(int iWert);
	void hp_aufladen(int iWert);
	void waffe_setzen(int Waffe) 
	{
		iWaffeAusgewaehlt=Waffe; 
		if (Waffe == 1) iWaffentimer = WAFFENTIMER_GROSSEWAFFE;
		if (Waffe == 2) iWaffentimer = WAFFENTIMER_KLEINEWAFFE;
	}
	int	 get_waffe(){return iWaffeAusgewaehlt;}
	int  get_waffentimer(){return iWaffentimer;}
	int  waffe_abfeuern();
	int  get_hp() {return iHp;}
	void render() {objekt::render(); iWaffentimer--; if(iWaffentimer<=0) iWaffeAusgewaehlt=0;}

};