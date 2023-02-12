#pragma once

#include"Enum.h"
#define SHIPCOUNTTYPE 4
#define LARGESTSHIPSIZE 5

class Ship
{
	Kind kind;
	int id;
	int x, y;
	Direction dir;
public:
	Ship();
	Ship(int y, int x, Direction d, Kind k, int id);
	Kind getKind();
	int getSize();
	int getX();
	int getY();
	Direction getDirection();
};

