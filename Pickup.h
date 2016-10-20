#pragma once
#include "SpaCE/applikation.h"
#include "Spieler.h"
#include "Attribute.h"

class Pickup : public objekt
{
private: 
	struct Pickupfilesystem pfstAttribute;
public:
	Pickup(objekt *copymodell, Pickupfilesystem *pfstAttr);
	void aktion(Spieler &sp);
	~Pickup(){};
};