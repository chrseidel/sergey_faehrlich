#include "Pickup.h"

void Pickup::aktion(Spieler &sp)
{

	switch(pfstAttribute.iModell)
	{
		case 0:
			sp.hp_aufladen(pfstAttribute.iWert);
			break;
		case 1:
			sp.waffe_setzen(pfstAttribute.iWert);
			break;
		case 2:
			sp.waffe_setzen(pfstAttribute.iWert);
			break;
		case 3:
			sp.waffe_setzen(pfstAttribute.iWert);
			break;
		case 4:
			break;
	}
}

Pickup::Pickup(objekt *copymodell, Pickupfilesystem *pfstAttr)
{
	objekt::operator=(*copymodell);
	pfstAttribute=*pfstAttr;
}