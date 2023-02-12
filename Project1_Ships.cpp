#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "Player.h"

#define COMMANDLENGTH 40
#define TAG_LENGTH 4

using namespace std;

void mergePrint(Player* currentPlayer, Player* A, Player* B) {  // Completing the array for printing
    for (int j = 0; j < currentPlayer->getBoard()->getX(); j++) {
        for (int i = 0; i < currentPlayer->getBoard()->getY(); i++) {
            if (A->getBoard()->getField(j, i)->getSymbol() != B->getBoard()->getField(j, i)->getSymbol() && (B->getBoard()->getField(j, i)->getSymbol() == '+' 
                || B->getBoard()->getField(j, i)->getSymbol() == '@' 
                || B->getBoard()->getField(j, i)->getSymbol() == '!' 
                || B->getBoard()->getField(j, i)->getSymbol() == '%')) {
                A->getBoard()->setField(j, i, B->getBoard()->getField(j, i));
            }
        }
    }
}

void print(int type, Player* currentPlayer, Player* A, Player* B) {
    if (type == 0) {
        if (currentPlayer->getBoard()->getX() == BOARDSIZEX && currentPlayer->getBoard()->getY() == BOARDSIZEY)
            currentPlayer->getBoard()->print();
        else {
            mergePrint(currentPlayer, A, B);
            A->getBoard()->print();
        }
    }
    else {
        if (currentPlayer->getBoard()->getX() == BOARDSIZEX && currentPlayer->getBoard()->getY() == BOARDSIZEY)
            currentPlayer->getBoard()->extendedPrint();
        else {
            mergePrint(currentPlayer, A, B);
            A->getBoard()->extendedPrint();
        }
    }
    cout << "PARTS REMAINING:: A : " << A->allShipsCount() - B->getHitCount() << " B : " << B->allShipsCount() - A->getHitCount() << "\n";
}

void zeroShotCount(Player* currentPlayer) {     // Set shot count to 0 for a new round
    for (int i = 0; i < currentPlayer->fullShipsCount(); i++) {
        currentPlayer->setShotCount(i);
    }
}

void setFleet(Player* A, Player* B, int a1, int a2, int a3, int a4, char P) {
    if (A->getPlayerSymbol() == P)
        A->initFleet(a1, a2, a3, a4);
    else
        B->initFleet(a1, a2, a3, a4);
}

void initBoard(Player* A, Player* B, int y, int x) {
    A->initBoard(y, x);
    B->initBoard(y, x);
}

void initPosition(char P, Player* A, Player* B, int x1, int x2, int y1, int y2) {
    if (P == 'A') {
        A->setStartingArea(x1, x2, y1, y2);
    }
    else B->setStartingArea(x1, x2, y1, y2);
}

void setHitCount(char* a, char P, Player* A, Player* B) {
    int i = 0;
    while (a[i] != '\0') {
        if (a[i] == '0') {
            if (P == 'A') {
                B->incHitCount();
            }
            else A->incHitCount();
        }
        i++;
    }
}

void setStartingPositions(Player* A, Player* B) {
    A->setStartY1();
    A->setStartY2();
    B->setStartY1();
    B->setStartY2();
    A->setStartX1();
    A->setStartX2();
    B->setStartX1();
    B->setStartX2();
}

int main() {
    Board* board = new Board();
    Player* A = new Player('A', board);
    Player* B = new Player('B', board);
    Player* currentPlayer = A;
    char lastTag = 'x';
    char* command = new char[COMMANDLENGTH];
    bool extendedShips = false;
    while (cin >> command) {
            if (strcmp(command, "[state]") == 0) {
                if (lastTag != 'x' && lastTag != 's') {
                    cout << "INVALID OPERATION " << '"' << command << ' ' << '"' << ": THE STATE EXPECTED";
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                if (lastTag == 'x') {
                    lastTag = 's';
                }
                else
                    lastTag = 'x';
                zeroShotCount(currentPlayer);
                continue;
            }

            if (strcmp(command, "[playerA]") == 0) {
                if (lastTag != 'x' && lastTag != 'a') {
                    cout << "INVALID OPERATION " << '"' << command << ' ' << '"' << ": THE OTHER PLAYER EXPECTED";
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                if (currentPlayer != A) {
                    cout << "INVALID OPERATION " << '"' << command << ' ' << '"' << ": THE OTHER PLAYER EXPECTED";
                    return false;
                }
                if (lastTag == 'x') {
                    lastTag = 'a';
                }
                else {
                    lastTag = 'x';
                        if (A->getHitCount() == B->allShipsCount() && B->allShipsCount() > 0) {
                            cout << "A won";
                            delete board;
                            delete A;
                            delete B;
                            delete[] command;
                            return false;
                        }
                    currentPlayer = B;
                }
                zeroShotCount(currentPlayer);
                continue;
            }

            if (strcmp(command, "[playerB]") == 0) {
                if (lastTag != 'x' && lastTag != 'b') {
                    cout << "INVALID OPERATION " << '"' << command << ' ' << '"' << ": THE OTHER PLAYER EXPECTED";
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                if (currentPlayer != B) {
                    cout << "INVALID OPERATION " << '"' << command << ' ' << '"' << ": THE OTHER PLAYER EXPECTED";
                    return false;
                }
                if (lastTag == 'x') {
                    lastTag = 'b';
                }
                else {
                    lastTag = 'x';
                        if (B->getHitCount() == A->allShipsCount() && A->allShipsCount() > 0) {
                            cout << "B won";
                            delete board;
                            delete A;
                            delete B;
                            delete[] command;
                            return false;
                        }
                    currentPlayer = A;
                }
                zeroShotCount(currentPlayer);
                continue;
            }

            if (strcmp(command, "PRINT") == 0) {
                int type;
                cin >> type;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                print(type, currentPlayer, A, B);
                continue;
            }

            if (strcmp(command, "SET_FLEET") == 0) {
                char P;
                int a1, a2, a3, a4;
                cin >> P >> a1 >> a2 >> a3 >> a4;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                setFleet(A, B, a1, a2, a3, a4, P);
                continue;
            }

            if (strcmp(command, "NEXT_PLAYER") == 0) {
                char P;
                cin >> P;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                if (P == 'A')
                    currentPlayer = A;
                else
                    currentPlayer = B;
                continue;
            }

            if (strcmp(command, "PLACE_SHIP") == 0) {
                int y, x, id;
                char d;
                char c[TAG_LENGTH];
                cin >> y >> x >> d >> id >> c;
                Direction dir;
                Kind kind;
                if (lastTag != 'a' && lastTag != 'b') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                if (d == 'N')
                    dir = UP;
                if (d == 'S')
                    dir = DOWN;
                if (d == 'W')
                    dir = LEFT;
                if (d == 'E')
                    dir = RIGHT;
                if (strcmp(c, "DES") == 0) {
                    kind = DES;
                }
                if (strcmp(c, "CRU") == 0) {
                    kind = CRU;
                }
                if (strcmp(c, "BAT") == 0) {
                    kind = BAT;
                }
                if (strcmp(c, "CAR") == 0) {
                    kind = CAR;
                }
                if (!currentPlayer->placeShip(y, x, dir, id, kind, command)) {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                else continue;
            }

            if (!extendedShips) {
                if (strcmp(command, "SHOOT") == 0) {
                    int y, x;
                    cin >> y >> x;
                    if (lastTag != 'a' && lastTag != 'b') {
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                    if (!A->checkAllShipsPlaced() || !B->checkAllShipsPlaced()) {
                        cout << "INVALID OPERATION " << '"' << command << ' ' << y << ' ' << x << '"' << ": NOT ALL SHIPS PLACED" << "\n";
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                    if (!currentPlayer->shoot(y, x, command)) {
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                    else continue;
                }
            }
            else {
                if (strcmp(command, "SHOOT") == 0) {
                    int y, x, id;
                    char c[TAG_LENGTH];
                    cin >> id >> c >> y >> x;
                    Kind kind;
                    if (strcmp(c, "DES") == 0) {
                        kind = DES;
                    }
                    if (strcmp(c, "CRU") == 0) {
                        kind = CRU;
                    }
                    if (strcmp(c, "BAT") == 0) {
                        kind = BAT;
                    }
                    if (strcmp(c, "CAR") == 0) {
                        kind = CAR;
                    }
                    if (lastTag != 'a' && lastTag != 'b') {
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                    if (!A->checkAllShipsPlaced() || !B->checkAllShipsPlaced()) {
                        cout << "INVALID OPERATION " << '"' << command << ' ' << y << ' ' << x << '"' << ": NOT ALL SHIPS PLACED" << "\n";
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                    if (!currentPlayer->shootExtended(y, x, id, kind, command)) {
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                    else {
                        currentPlayer->incShotCount(id);
                        continue;
                    }
                }
            }

            if (strcmp(command, "BOARD_SIZE") == 0) {
                int y, x;
                cin >> y >> x;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                initBoard(A, B, y, x);
                setStartingPositions(A, B);
                continue;
            }

            if (strcmp(command, "INIT_POSITION") == 0) {
                char P;
                int y1, y2, x1, x2;
                cin >> P >> y1 >> x1 >> y2 >> x2;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                initPosition(P, A, B, x1, x2, y1, y2);
                continue;
            }

            if (strcmp(command, "REEF") == 0) {
                int y, x;
                cin >> y >> x;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                currentPlayer->setReef(y, x, command);
                continue;
            }

            if (strcmp(command, "SHIP") == 0) {
                char P, d;
                int y, x, id;
                char c[TAG_LENGTH];
                char a[LARGESTSHIPSIZE + 1];
                cin >> P >> y >> x >> d >> id >> c >> a;
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                Direction dir;
                Kind kind;
                if (d == 'N')
                    dir = UP;
                if (d == 'S')
                    dir = DOWN;
                if (d == 'W')
                    dir = LEFT;
                if (d == 'E')
                    dir = RIGHT;
                if (strcmp(c, "DES") == 0) {
                    kind = DES;
                }
                if (strcmp(c, "CRU") == 0) {
                    kind = CRU;
                }
                if (strcmp(c, "BAT") == 0) {
                    kind = BAT;
                }
                if (strcmp(c, "CAR") == 0) {
                    kind = CAR;
                }
                setHitCount(a, P, A, B);
                if (P == 'A') {
                    if (!A->placePredefinedShip(y, x, dir, id, kind, command, a)) {
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                }
                else {
                    if (!B->placePredefinedShip(y, x, dir, id, kind, command, a)) {
                        delete board;
                        delete A;
                        delete B;
                        delete[] command;
                        return false;
                    }
                }
                continue;
            }

            if (strcmp(command, "EXTENDED_SHIPS") == 0) {
                if (lastTag != 's') {
                    delete board;
                    delete A;
                    delete B;
                    delete[] command;
                    return false;
                }
                extendedShips = true;
                continue;
            }
        }
    return 0;
}