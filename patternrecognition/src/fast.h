/**
 * Gives access to the algorithm that lines collinear points
 * @file fast.h
 * @authors vikho305 & isaho220
 */

#ifndef FAST_H
#define FAST_H

#include <QApplication>
#include <QGraphicsScene>
#include <vector>
#include "Point.h"

void lineCollinearPoints(QApplication &app, QGraphicsScene *scene, vector<Point> &points);

#endif // FAST_H
