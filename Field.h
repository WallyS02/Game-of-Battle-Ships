#pragma once

#include"Enum.h"

class Field {
	char symbol;
	Kind kind;
	int id;
public:
	Field();
	Field(char s, Kind k, int i);
	char getSymbol();
	Kind getKind();
	void setSymbol(char s);
};