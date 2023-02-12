#pragma once

#include"Ship.h"
#include"Board.h"

class Player
{
	char playerSymbol;
	int* shipCount;
	Ship*** ships;
	int hitCount;
	int* shotCount;
	Board* board;
	int* maxShipCount;
	int startX1;
	int startX2;
	int startY1;
	int startY2;
	bool checkShipPlace(int y, int x, Direction d, int id, Kind c, char* command, bool startingPosition, char* segments);
	bool checkValidField(int x, int y, Direction d, int id, Kind c, char* command, bool startingPosition, int shipHeadX, int shipHeadY, char* segments);
	bool checkShipCoordinates(int x, int y, bool startingPosition);
	bool checkShootCoordinates(int x, int y);
	bool checkExtendedShootCoordinates(int y, int x, int id, Kind c, char* command);
	char directionPrint(Direction d);
	void kindPrint(Kind c);
	void placeUp(int y, int x, Direction d, int id, Kind c, int i);
	void placeDown(int y, int x, Direction d, int id, Kind c, int i);
	void placeRight(int y, int x, Direction d, int id, Kind c, int i);
	void placeLeft(int y, int x, Direction d, int id, Kind c, int i);
public:
	Player();
	Player(char s, Board* board);
	~Player();
	bool checkAllShipsPlaced();
	void initFleet(int a1, int a2, int a3, int a4);
	void initBoard(int y, int x);
	bool placeShip(int y, int x, Direction d, int id, Kind c, char* command);
	bool placePredefinedShip(int y, int x, Direction d, int id, Kind c, char* command, char* segments);
	bool shoot(int y, int x, char* command);
	void setStartingArea(int x1, int x2, int y1, int y2);
	bool setReef(int y, int x, char* command);
	int allShipsCount();
	int fullShipsCount();
	int getHitCount();
	void incShotCount(int id);
	void setStartY1();
	void setStartY2();
	void setStartX1();
	void setStartX2();
	char getPlayerSymbol();
	Board* getBoard();
	void incHitCount();
	void setShotCount(int id);
	bool shootExtended(int y, int x, int id, Kind c, char* command);
};