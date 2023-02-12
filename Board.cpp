#pragma once

#include "Board.h"
#include <iostream>

using namespace std;

Board::Board() {
	x = BOARDSIZEX;
	y = BOARDSIZEY;
	field = new Field **[x];
	for (int i = 0; i < x; i++) {
		field[i] = new Field*[y];
		for (int j = 0; j < y; j++) {
			field[i][j] = new Field();
		}
	}
}

Board::Board(int x, int y){
	this->x = x;
	this->y = y;
	field = new Field **[x];
	for (int i = 0; i < x; i++) {
		field[i] = new Field * [y];
		for (int j = 0; j < y; j++) {
			field[i][j] = new Field();
		}
	}
}

Board::~Board() {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			if (field[i][j] != nullptr) {
				delete field[i][j];
			}
		}
	}
	for (int i = 0; i < x; i++) {
		if(field && field[i])
			delete[] field[i];
	}
	if(field)
		delete[] field;
}

void Board::print() {
	for (int j = 0; j < y; j++) {
		for (int i = 0; i < x; i++) {
			if (field[i][j]->getSymbol() != ' ' && field[i][j]->getSymbol() != '#' && field[i][j]->getSymbol() != 'x') {
				cout << "+";
			}
			else cout << field[i][j]->getSymbol();
		}
		cout << "\n";
	}
}

int Board::getX() {
	return x;
}

int Board::getY() {
	return y;
}

Field* Board::getField(int i, int j) {
	return field[i][j];
}

void Board::setField(int i, int j, Field* f) {
	field[i][j] = f;
}

void Board::extendedPrint() {
	cout << "  ";
	int p = 0;
	for (int i = 0; i < x; i++) {
		if (i % 10 == 0 && i != 0) {
			p++;
		}
		cout << p;
	}
	cout << "\n";
	cout << "  ";
	for (int i = 0; i < x; i++) {
		cout << i%10;
	}
	cout << "\n";
	for (int j = 0; j < y; j++) {
		if (j < 10) {
			cout << '0' << j;
		}
		else cout << j;
		for (int i = 0; i < x; i++) {
			cout << field[i][j]->getSymbol();
		}
		cout << "\n";
	}
}