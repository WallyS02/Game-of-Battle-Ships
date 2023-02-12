#include"Field.h"

Field::Field() {
	symbol = ' ';
	kind = NONE;
	id = -1;
}

Field::Field(char s, Kind k, int i) {
	symbol = s;
	kind = k;
	id = i;
}

char Field::getSymbol() {
	return symbol;
}
Kind Field::getKind() {
	return kind;
}
void Field::setSymbol(char s) {
	symbol = s;
}