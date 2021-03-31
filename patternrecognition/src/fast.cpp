/**
 * Defines all methods related to quickly lining collinear points
 * @file fast.cpp
 * @authors vikho305 & isaho220
 */

#include "fast.h"
#include <queue>
#include <vector>

bool pointSlopeCmp(const pair<Point, double>& a, const pair<Point, double>& b) {
    return a.second < b.second;
}

void lineCollinearFromOrigin(QGraphicsScene *scene, Point& origin, const vector<Point> &points)
{
    vector<pair<Point, double>> pointSlopes;

    // Pair all slopes with respective points
    for(const Point &point : points)
    {
        if (&point != &origin)
        {
            double slope = origin.slopeTo(point);
            pointSlopes.push_back(make_pair(point, slope));
        }
    }
    sort(pointSlopes.begin(), pointSlopes.end(), pointSlopeCmp);

    // Draw all lines over collinear points
    uint counter = 1;
    double prevSlope = origin.slopeTo(origin); // Set to infinity (unlikely slope value)
    Point prevPoint = origin; // Point to draw next line from
    vector<Point> prevPoints; // Keeps track of which points to line (acts as pseudo-queue)

    for(pair<Point, double> &pointSlope : pointSlopes)
    {
        Point point = pointSlope.first;
        double slope = pointSlope.second;

        if(slope == prevSlope)
        {
            counter++;
            prevPoints.insert(prevPoints.begin(), point);

            if(counter >= 4)
            {
                while(!prevPoints.empty())
                {
                    Point nextPoint = prevPoints.back();
                    prevPoints.pop_back();
                    prevPoint.lineTo(scene, nextPoint);
                    prevPoint = nextPoint;
                }
            }
        }
        else
        {
            prevSlope = slope;
            prevPoint = origin;
            counter = 2; // 2 to account for new point & origin

            prevPoints.clear();
            prevPoints.push_back(point);
        }
    }
}

void lineCollinearPoints(QApplication &app, QGraphicsScene *scene, vector<Point> &points)
{
    if(points.size() >= 4) // Skips if 4 possibly collinear points don't exist
    {
        for(Point &origin : points)
        {
            lineCollinearFromOrigin(scene, origin, points);
            app.processEvents();
        }
    }
}
