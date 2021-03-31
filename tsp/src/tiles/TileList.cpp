/**
 * Defines all functionality of the TileList class, which stores instances of Tile.
 * @file TourList.cpp
 * @authors vikho305 & isaho220
 */

#include "TileList.h"

TileList::TileList()
{
    _arraySize = 10;
    _tileAmount = 0;

    _tileArr = new Tile[_arraySize];
}

TileList::~TileList()
{
    delete [] _tileArr;
}

/**
 * Adds tile to the top of the list
 */
void TileList::addTile(Tile tile)
{
    if(_tileAmount >= _arraySize)
    {
        Tile* newTileArr = new Tile[_arraySize*_arraySize];
        for(int i = 0; i < _arraySize; i++)
            newTileArr[i] = _tileArr[i];
        _arraySize =  _arraySize * _arraySize;
        delete [] _tileArr;
        _tileArr = newTileArr;
    }
    _tileArr[_tileAmount] = tile;
    _tileAmount++;
}

/**
 * Draws all tiles in given scene
 */
void TileList::drawAll(QGraphicsScene* scene)
{
    scene->clear();

    for(int i = 0; i < _tileAmount; i++)
    {
        _tileArr[i].draw(scene);
    }
}

/**
 * Returns tile index of tile covering given x, y position
 * @param xPos
 * @param yPos
 * @return tile index
 */
int TileList::indexOfTopTile(int x, int y)
{
    int index = -1;
    for(int i = 0; i < _tileAmount; i++)
    {
        int x1 = _tileArr[i].x;
        int x2 = _tileArr[i].x + _tileArr[i].width;

        int y1 = _tileArr[i].y;
        int y2 = _tileArr[i].y + _tileArr[i].height;

        if((x >= x1 && x <= x2) && (y >= y1 && y <= y2))
        {
            index = i;
        }
    }
    return index;
}

/**
 * Moves clicked tile to top
 * @param xPos
 * @param yPos
 */
void TileList::raise(int x, int y)
{
    int index = indexOfTopTile(x, y);

    if(index != -1)
    {
        while(index < _tileAmount-1)
        {
            Tile tile = _tileArr[index];
            _tileArr[index] = _tileArr[index+1];
            _tileArr[index+1] = tile;
            index++;
        }
    }
}

/**
 * Moves clicked tile to bottom
 * @param xPos
 * @param yPos
 */
void TileList::lower(int x, int y)
{
    int index = indexOfTopTile(x, y);

    if(index != -1)
    {
        while(index > 0)
        {
            Tile tile = _tileArr[index];
            _tileArr[index] = _tileArr[index-1];
            _tileArr[index-1] = tile;
            index--;
        }
    }
}

/**
 * Removes clicked tile
 * @param xPos
 * @param yPos
 */
void TileList::remove(int x, int y)
{
    int index = indexOfTopTile(x, y);

    if(index != -1)
    {
        while(index < _tileAmount-1)
        {
            _tileArr[index] = _tileArr[index+1];
            index++;
        }
        _tileAmount--;
    }
}

/**
 * Removes all tiles covering given x, y position
 * @param x
 * @param y
 */
void TileList::removeAll(int x, int y)
{
    int index = indexOfTopTile(x, y);
    do
    {
        index = indexOfTopTile(x, y);
        remove(x, y);
    }
    while(index != -1);
}
