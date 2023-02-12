#pragma once

#include"Enum.h"
#include"Field.h"

#define BOARDSIZEX 10
#define BOARDSIZEY 21

class Board
{
	Field*** field;
	int x, y;
public:
	Board();
	Board(int x, int y);
	~Board();
	void print();
	void extendedPrint();
	int getX();
	int getY();
	Field* getField(int i, int j);
	void setField(int i, int j, Field* f);
};