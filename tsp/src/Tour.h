/**
 * Declares all the functionality and variables of the Tour class
 * @file Tour.h
 * @authors vikho305 & isaho220
 */

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:
    Tour();
    ~Tour();
    void show();
    void draw(QGraphicsScene* scene);
    int size();
    double distance();
    void insertNearest(Point p);
    void insertSmallest(Point p);

private:
    Node* _startNode;
};

#endif // TOUR_H
