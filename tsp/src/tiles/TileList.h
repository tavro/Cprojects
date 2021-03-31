/**
 * Declares all functionality and variables of the TileList class, which stores instances of Tile.
 * @file TourList.h
 * @authors vikho305 & isaho220
 */

#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

class TileList {
public:
    TileList();
    ~TileList();
    void addTile(Tile tile);
    void drawAll(QGraphicsScene* scene);
    int indexOfTopTile(int x, int y);
    void lower(int x, int y);
    void raise(int x, int y);
    void remove(int x, int y);
    void removeAll(int x, int y);

private:
    int _tileAmount;
    int _arraySize;
    Tile* _tileArr;
};

#endif // TILELIST_H
