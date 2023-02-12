#pragma once

#include "Player.h"
#include <iostream>
#include <cmath>

#define KIND_LENGTH 3

using namespace std;

Player::Player() {
	playerSymbol = ' ';
	shipCount = nullptr;
	ships = nullptr;
	hitCount = 0;
	shotCount = nullptr;
	board = nullptr;
	maxShipCount = nullptr;
	startX1 = 0;
	startX2 = 0;
	startY1 = 0;
	startY2 = 0;
}

Player::Player(char s, Board* board) {
	playerSymbol = s;
	shipCount = nullptr;
	ships = nullptr;
	hitCount = 0;
	shotCount = nullptr;
	this->board = board;
	if (playerSymbol == 'A') {
		setStartingArea(0, BOARDSIZEX - 1, 0, BOARDSIZEY / 2 - 1);
	}
	else {
		setStartingArea(0, BOARDSIZEX - 1, (BOARDSIZEY / 2) + 1, BOARDSIZEY - 1);
	}
	maxShipCount = nullptr;
	initFleet(1, 2, 3, 4);
}

Player::~Player() {
	for (int i = 0; i < SHIPCOUNTTYPE; i++) {
		for (int j = 0; j < maxShipCount[i]; j++) {
			if(ships[i][j])
				delete ships[i][j];
		}
	}
	for (int i = 0; i < SHIPCOUNTTYPE; i++) {
		delete[] ships[i];
	}
	delete[] shipCount;
	delete[] maxShipCount;
	delete[] shotCount;
	delete[] ships;
}

int Player::allShipsCount() {	// Count live parts of all ships owned by player
	int sum = 0;
	for (int i = LARGESTSHIPSIZE; i >= LARGESTSHIPSIZE - SHIPCOUNTTYPE + 1; i--) {
		sum += i * shipCount[LARGESTSHIPSIZE - i];
	}
	return sum;
}

int Player::fullShipsCount() {	// Count number of ships owned by player
	int sum = 0;
	for (int i = LARGESTSHIPSIZE; i >= LARGESTSHIPSIZE - SHIPCOUNTTYPE + 1; i--) {
		sum += shipCount[LARGESTSHIPSIZE - i];
	}
	return sum;
}

char Player::directionPrint(Direction d) {	// Direction conversion to char type
	char dir;
	if (d == UP)
		dir = 'N';
	if (d == DOWN)
		dir = 'S';
	if (d == LEFT)
		dir = 'W';
	if (d == RIGHT)
		dir = 'E';
	return dir;
}

void Player::kindPrint(Kind c) {	// Kind conversion to c-string type
	if (c == DES) {
		cout << "DES";
	}
	if (c == CRU) {
		cout << "CRU";
	}
	if (c == BAT) {
		cout << "BAT";
	}
	if (c == CAR) {
		cout << "CAR";
	}
}

bool Player::checkShipPlace(int y, int x, Direction d, int id, Kind c, char* command, bool startingPosition, char* segments) {
	if (ships[c][id] && id < maxShipCount[c]) {
		if (startingPosition == false) {
			cout << "INVALID OPERATION " << '"' << command << " " << playerSymbol << " " << y << " " << x << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << " " << segments << '"' << ": SHIP ALREADY PRESENT" << "\n";
		}
		else {
			cout << "INVALID OPERATION " << '"' << command << " " << y << " " << x << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << '"' << ": SHIP ALREADY PRESENT" << "\n";
		}
		return false;
	}
	if (shipCount[c] == maxShipCount[c]) {
		if (startingPosition == false) {
			cout << "INVALID OPERATION " << '"' << command << " " << playerSymbol << " " << y << " " << x << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << " " << segments << '"' << ": ALL SHIPS OF THE CLASS ALREADY SET" << "\n";
		}
		else {
			cout << "INVALID OPERATION " << '"' << command << " " << y << " " << x << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << '"' << ": ALL SHIPS OF THE CLASS ALREADY SET" << "\n";
		}
		return false;
	}

	int shipHeadX = x, shipHeadY = y;
	for (int i = 0; i < LARGESTSHIPSIZE - c; i++) {
		switch (d)
		{
		case UP:
			if(!checkValidField(x, y + i, d, id, c, command, startingPosition, shipHeadX, shipHeadY, segments)) return false;
			break;
		case DOWN:
			if (!checkValidField(x, y - i, d, id, c, command, startingPosition, shipHeadX, shipHeadY, segments)) return false;
			break;
		case RIGHT:
			if (!checkValidField(x - i, y, d, id, c, command, startingPosition, shipHeadX, shipHeadY, segments)) return false;
			break;
		case LEFT:
			if (!checkValidField(x + i, y, d, id, c, command, startingPosition, shipHeadX, shipHeadY, segments)) return false;
			break;
		}
	}
	return true;
}

bool Player::checkShipCoordinates(int x, int y, bool startingPosition) {
	if (startingPosition) {
		if (!(startX2 >= x && x >= startX1)) {
			return false;
		}
		if (!(startY2 >= y && y >= startY1)) {
			return false;
		}
	}
	else {
		if (!(board->getX() - 1 >= x && x >= 0)) {
			return false;
		}
		if (!(board->getY() - 1 >= y && y >= 0)) {
			return false;
		}
	}
	return true;
}

bool Player::checkValidField(int x, int y, Direction d, int id, Kind c, char* command, bool startingPosition, int shipHeadX, int shipHeadY, char* segments) {
	if (!checkShipCoordinates(x, y, startingPosition)) {
		if (startingPosition == false) {
			cout << "INVALID OPERATION " << '"' << command << " " << playerSymbol << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << " " << segments << '"' << ": NOT IN STARTING POSITION" << "\n";
		}
		else {
			cout << "INVALID OPERATION " << '"' << command << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << '"' << ": NOT IN STARTING POSITION" << "\n";
		}
		return false;
	}
	if (board->getField(x, y)->getKind() == REEF) {
		if (startingPosition == false) {
			cout << "INVALID OPERATION " << '"' << command << " " << playerSymbol << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << " " << segments << '"' << ": PLACING SHIP ON REEF" << "\n";
		}
		else {
			cout << "INVALID OPERATION " << '"' << command << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << '"' << ": PLACING SHIP ON REEF" << "\n";
		}
		return false;
	}
	if (board->getField(x, y)->getKind() != NONE) {
		if (startingPosition == false) {
			cout << "INVALID OPERATION " << '"' << command << " " << playerSymbol << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << " " << segments << '"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << "\n";
		}
		else {
			cout << "INVALID OPERATION " << '"' << command << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << '"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << "\n";
		}
		return false;
	}
	if ((checkShipCoordinates(x + 1, y, startingPosition) && board->getField(x + 1, y)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x + 1, y + 1, startingPosition) && board->getField(x + 1, y + 1)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x, y + 1, startingPosition) && board->getField(x, y + 1)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x - 1, y + 1, startingPosition) && board->getField(x - 1, y + 1)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x - 1, y, startingPosition) && board->getField(x - 1, y)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x - 1, y - 1, startingPosition) && board->getField(x - 1, y - 1)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x, y - 1, startingPosition) && board->getField(x, y - 1)->getKind() < SHIPCOUNTTYPE)
		|| (checkShipCoordinates(x + 1, y - 1, startingPosition) && board->getField(x + 1, y - 1)->getKind() < SHIPCOUNTTYPE)) {
		if (startingPosition == false) {
			cout << "INVALID OPERATION " << '"' << command << " " << playerSymbol << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << " " << segments << '"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << "\n";
		}
		else {
			cout << "INVALID OPERATION " << '"' << command << " " << shipHeadY << " " << shipHeadX << " " << directionPrint(d) << " " << id << " ";
			kindPrint(c);
			cout << '"' << ": PLACING SHIP TOO CLOSE TO OTHER SHIP" << "\n";
		}
		return false;
	}
	return true;
}

void Player::placeUp(int y, int x, Direction d, int id, Kind c, int i){
	if (ships[c][id]->getKind() != DES) {
		if (i == 0) {
			board->setField(x, y + i, new Field('@', c, id));
		}
		else if (i == 1) {
			board->setField(x, y + i, new Field('!', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x, y + i, new Field('%', c, id));
		}
		else board->setField(x, y + i, new Field('+', c, id));
	}
	else {
		if (i == 0) {
			board->setField(x, y + i, new Field('@', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x, y + i, new Field('%', c, id));
		}
	}
}

void Player::placeDown(int y, int x, Direction d, int id, Kind c, int i) {
	if (ships[c][id]->getKind() != DES) {
		if (i == 0) {
			board->setField(x, y - i, new Field('@', c, id));
		}
		else if (i == 1) {
			board->setField(x, y - i, new Field('!', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x, y - i, new Field('%', c, id));
		}
		else board->setField(x, y - i, new Field('+', c, id));
	}
	else {
		if (i == 0) {
			board->setField(x, y - i, new Field('@', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x, y - i, new Field('%', c, id));
		}
	}
}

void Player::placeRight(int y, int x, Direction d, int id, Kind c, int i) {
	if (ships[c][id]->getKind() != DES) {
		if (i == 0) {
			board->setField(x - i, y, new Field('@', c, id));
		}
		else if (i == 1) {
			board->setField(x - i, y, new Field('!', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x - i, y, new Field('%', c, id));
		}
		else board->setField(x - i, y, new Field('+', c, id));
	}
	else {
		if (i == 0) {
			board->setField(x - i, y, new Field('@', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x - i, y, new Field('%', c, id));
		}
	}
}

void Player::placeLeft(int y, int x, Direction d, int id, Kind c, int i) {
	if (ships[c][id]->getKind() != DES) {
		if (i == 0) {
			board->setField(x + i, y, new Field('@', c, id));
		}
		else if (i == 1) {
			board->setField(x + i, y, new Field('!', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x + i, y, new Field('%', c, id));
		}
		else board->setField(x + i, y, new Field('+', c, id));
	}
	else {
		if (i == 0) {
			board->setField(x + i, y, new Field('@', c, id));
		}
		else if (i == ships[c][id]->getSize() - 1) {
			board->setField(x + i, y, new Field('%', c, id));
		}
	}
}

bool Player::placeShip(int y, int x, Direction d, int id, Kind c, char* command) {
	if (!checkShipPlace(y, x, d, id, c, command, true, nullptr)) {
		return false;
	}
	Ship* s = new Ship(y, x, d, c, id);
	ships[c][id] = s;
	shipCount[c]++;
	for (int i = 0; i < ships[c][id]->getSize(); i++) {
		switch (d)
		{
		case UP:
			placeUp(y, x, d, id, c, i);
			break;
		case DOWN:
			placeDown(y, x, d, id, c, i);
			break;
		case RIGHT:
			placeRight(y, x, d, id, c, i);
			break;
		case LEFT:
			placeLeft(y, x, d, id, c, i);
			break;
		}
	}
	return true;
}

bool Player::placePredefinedShip(int y, int x, Direction d, int id, Kind c, char* command, char* segments) {
	if (!checkShipPlace(y, x, d, id, c, command, false, segments)) {
		return false;
	}
	Ship* s = new Ship(y, x, d, c, id);
	ships[c][id] = s;
	shipCount[c]++;
	for (int i = 0; i < ships[c][id]->getSize(); i++) {
		if (ships[c][id]->getSize() != 2) {
			if (segments[i] == '0') {
				segments[i] = 'x';
			}
			else if (segments[i] != '0' && i == 0) {
				segments[i] = '@';
			}
			else if (segments[i] != '0' && i == 1) {
				segments[i] = '!';
			}
			else if (segments[i] != '0' && i == ships[c][id]->getSize() - 1) {
				segments[i] = '%';
			}
			else segments[i] = '+';
		}
		else {
			if (segments[i] == '0') {
				segments[i] = 'x';
			}
			else if (segments[i] != '0' && i == 0) {
				segments[i] = '@';
			}
			else if (segments[i] != '0' && i == ships[c][id]->getSize() - 1) {
				segments[i] = '%';
			}
		}
		switch (d)
		{
		case UP:
			board->setField(x, y + i, new Field(segments[i], c, id));
			break;
		case DOWN:
			board->setField(x, y - i, new Field(segments[i], c, id));
			break;
		case RIGHT:
			board->setField(x - i, y, new Field(segments[i], c, id));
			break;
		case LEFT:
			board->setField(x + i, y, new Field(segments[i], c, id));
			break;
		}
	}
	return true;
}

bool Player::checkAllShipsPlaced() {
	for (int i = 0; i < SHIPCOUNTTYPE; i++) {
		if (shipCount[i] != maxShipCount[i]) {
			return false;
		}
	}
	return true;
}

bool Player::checkShootCoordinates(int x, int y) {
	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= board->getX()) return false;
	if (y >= board->getY()) return false;
	return true;
}

bool Player::shoot(int y, int x, char* command) {
	if (!checkShootCoordinates(x, y)) {
		cout << "INVALID OPERATION " << '"' << command << ' ' << y << ' ' << x << '"' << ": FIELD DOES NOT EXIST" << "\n";
		return false;
	}
	if (board->getField(x, y)->getKind() < SHIPCOUNTTYPE && board->getField(x, y)->getSymbol() != 'x') {
		board->getField(x, y)->setSymbol('x');
		hitCount++;
	}
	return true;
}

void Player::initFleet(int a1, int a2, int a3, int a4) {
	if (shipCount) {
		delete[] shipCount;
	}
	if (maxShipCount) {
		delete[] maxShipCount;
	}
	if (shotCount) {
		delete[] shotCount;
	}
	shipCount = new int[SHIPCOUNTTYPE];
	maxShipCount = new int[SHIPCOUNTTYPE];
	shotCount = new int[a1 + a2 + a3 + a4];
	maxShipCount[0] = a1;
	maxShipCount[1] = a2;
	maxShipCount[2] = a3;
	maxShipCount[3] = a4;
	for (int i = 0; i < SHIPCOUNTTYPE; i++) {
		shipCount[i] = 0;
	}
	for (int i = 0; i < a1 + a2 + a3 + a4; i++) {
		shotCount[i] = 0;
	}
	ships = new Ship** [SHIPCOUNTTYPE];
	for (int i = 0; i < SHIPCOUNTTYPE; i++) {
		ships[i] = new Ship* [maxShipCount[i]];
		for (int j = 0; j < maxShipCount[i]; j++) {
			ships[i][j] = nullptr;
		}
	}
}

void Player::initBoard(int y, int x) {
	board = new Board(x, y);
}

void Player::setStartingArea(int x1, int x2, int y1, int y2) {
	startX1 = x1;
	startX2 = x2;
	startY1 = y1;
	startY2 = y2;
}

bool Player::setReef(int y, int x, char* command) {
	if (!checkShootCoordinates(x, y)) {
		cout << "INVALID OPERATION " << '"' << command << ' ' << y << ' ' << x << '"' << ": REEF IS NOT PLACED ON BOARD" << "\n";
		return false;
	}
	board->setField(x, y, new Field('#', REEF, 0));
	return true;
}

char Player::getPlayerSymbol() {
	return playerSymbol;
}

Board* Player::getBoard() {
	return board;
}

int Player::getHitCount() {
	return hitCount;
}

void Player::incHitCount() {
	hitCount++;
}

void Player::incShotCount(int id) {
	shotCount[id]++;
}

bool Player::checkExtendedShootCoordinates(int y, int x, int id, Kind c, char* command) {
	if (shotCount[id] >= LARGESTSHIPSIZE - c) {
		cout << "INVALID OPERATION " << '"' << command << ' ' << id << ' ';
		kindPrint(c);
		cout << ' ' << x << ' ' << y << '"' << ": TOO MANY SHOOTS" << "\n";
		return false;
	}
	int x1 = 0, y1 = 0;
	switch (ships[c][id]->getDirection())
	{
	case UP:
		x1 = ships[c][id]->getX();
		y1 = ships[c][id]->getY() + 1;
		break;
	case DOWN:
		x1 = ships[c][id]->getX();
		y1 = ships[c][id]->getY() - 1;
		break;
	case LEFT:
		x1 = ships[c][id]->getX() + 1;
		y1 = ships[c][id]->getY();
		break;
	case RIGHT:
		x1 = ships[c][id]->getX() - 1;
		y1 = ships[c][id]->getY();
		break;
	}
	if (board->getField(x1, y1)->getSymbol() == 'x') {
		cout << "INVALID OPERATION " << '"' << command << ' ' << id << ' ';
		kindPrint(c);
		cout << ' ' << x << ' ' << y << '"' << ": SHIP CANNOT SHOOT" << "\n";
		return false;
	}
	int w1 = abs(y1 - x);
	int w2 = abs(x1 - y);
	int p1 = pow(w1, 2);
	int p2 = pow(w2, 2);
	if (c != CAR) {
		if (sqrt(p1 + p2) > ships[c][id]->getSize()) {
			cout << "INVALID OPERATION " << '"' << command << ' ' << id << ' ';
			kindPrint(c);
			cout << ' ' << x << ' ' << y << '"' << ": SHOOTING TOO FAR" << "\n";
			return false;
		}
	}
	return true;
}

void Player::setShotCount(int id) {
	shotCount[id] = 0;
}

bool Player::shootExtended(int y, int x, int id, Kind c, char* command) {
	if (!checkShootCoordinates(x, y)) {
		cout << "INVALID OPERATION " << '"' << command << ' ' << y << ' ' << x << '"' << ": FIELD DOES NOT EXIST" << "\n";
		return false;
	}
	if (!checkExtendedShootCoordinates(x, y, id, c, command)) {
		return false;
	}
	if (board->getField(x, y)->getKind() < SHIPCOUNTTYPE && board->getField(x, y)->getSymbol() != 'x') {
		board->getField(x, y)->setSymbol('x');
		hitCount++;
	}
	return true;
}

void Player::setStartY1() {
	startY1 = 0;
}
void Player::setStartY2() {
	startY2 = board->getY();
}

void Player::setStartX1() {
	startX1 = 0;
}
void Player::setStartX2() {
	startX2 = board->getX();
}