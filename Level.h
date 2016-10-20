#include <list>
#pragma once

using namespace std;

struct spawn
{
	int iGegnertyp;
	int iFormation;
	int iPosition;
};

class Level
{
private:
	list<spawn *> lstSpawnpunkt;
	list<spawn *>::iterator current_enemy;

	int iSizeOfList;

public:
	int load_level(int level);
	void get_next_enemy(int *Gegnertyp, int *Formation, int *Position);
	int randomPosition(int Grenze);
	Level(int Level = 0);
	~Level();
};
