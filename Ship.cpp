#include "Ship.h"

Ship::Ship() {
	kind = NONE;
	id = -1;
	x = -1;
	y = -1;
	dir = NON;
}

Ship::Ship(int y, int x, Direction d, Kind k, int id) {
	kind = k;
	this->id = id;
	this->x = x;
	this->y = y;
	dir = d;
}

int Ship::getSize() {
	return LARGESTSHIPSIZE - kind;
}

int Ship::getX() {
	return x;
}

int Ship::getY() {
	return y;
}

Kind Ship::getKind() {
	return kind;
}

Direction Ship::getDirection() {
	return dir;
}