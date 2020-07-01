#ifndef BOARD_H
#define BOARD_H

void placeBlinker(bool gameWorld[60][80], int row, int column);
void placeGlider(bool gameWorld[60][80], int row, int column);
void placeGun(bool gameWorld[60][80], int row, int column);
void copyWorld(bool copyFrom[60][80], bool copyTo[60][80]);
void simulateWorld(bool gameWorld[60][80]);
void printWorld(bool gameWorld[60][80]);

#endif // BOARD_H
