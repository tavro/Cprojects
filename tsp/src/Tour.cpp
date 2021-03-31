/**
 * Defines all the functionality of the tour class
 * @file Tour.cpp
 * @authors vikho305 & isaho220
 */

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour()
{
    _startNode = nullptr;
}

Tour::~Tour()
{
    Node* currentNode = _startNode;
    int currentSize = size();

    for (int i = 0; i < currentSize; i++)
    {
        Node* nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
}

/**
 * Prints all the points of the tour to the console
 */
void Tour::show()
{
    Node* currentNode = _startNode;
    do
    {
        std::cout << currentNode->point.toString() << std::endl;
        currentNode = currentNode->next;
    }
    while(currentNode != _startNode);
}

/**
 * Draws lines between all the nodes in the tour
 * @param scene - The scene in which to draw
 */
void Tour::draw(QGraphicsScene *scene)
{
    scene->clear();

    Node* currentNode = _startNode;
    do
    {
        Node* nextNode = currentNode->next;
        currentNode->point.drawTo(nextNode->point, scene);
        currentNode = nextNode;

    }
    while(currentNode != _startNode);
}

/**
 * Returns the amount of nodes in the tour
 * @return The number of nodes in the tour
 */
int Tour::size()
{
    int size = 0;

    Node* currentNode = _startNode;
    do
    {
        size++;
        currentNode = currentNode->next;
    }
    while(currentNode != _startNode);

    return size;
}

/**
 * Returns the sum of the distance between all neighbouring nodes in the tour
 * @return The sum of all distances between neighbouring nodes
 */
double Tour::distance()
{
    double distance = 0;

    Node* currentNode = _startNode;
    do
    {
        Node* nextNode = currentNode->next;
        distance += currentNode->point.distanceTo(nextNode->point);
        currentNode = nextNode;

    }
    while(currentNode != _startNode);

    return distance;
}

/**
 * Inserts a point into the tour based on which node is nearest
 * @param p - The point to insert
 */
void Tour::insertNearest(Point p)
{
    if (_startNode != nullptr)
    {
        Node* nearestNode = _startNode;
        double nearestDistance = -1;

        Node* currentNode = _startNode;
        do
        {
            double distance = currentNode->point.distanceTo(p);
            if (nearestDistance < 0 || nearestDistance > distance)
            {
                nearestDistance = distance;
                nearestNode = currentNode;
            }

            currentNode = currentNode->next;

        }
        while(currentNode != _startNode);

        Node* nextNode = nearestNode->next;
        nearestNode->next = new Node(p);
        nearestNode->next->next = nextNode;
    }
    else
    {
        _startNode = new Node(p);
        _startNode->next = _startNode;
    }
}

/**
 * Inserts a point into the tour based on where it increases the distance the least
 * @param p - The point to insert
 */
void Tour::insertSmallest(Point p)
{
    if (_startNode != nullptr)
    {
        Node* nearestNode = _startNode;
        double smallestIncrease = -1;

        Node* currentNode = _startNode;
        do
        {
            double originalDist = currentNode->point.distanceTo(currentNode->next->point);
            double distance = currentNode->point.distanceTo(p) + currentNode->next->point.distanceTo(p);
            if (smallestIncrease < 0 || smallestIncrease > distance - originalDist)
            {
                smallestIncrease = distance - originalDist;
                nearestNode = currentNode;
            }

            currentNode = currentNode->next;
        }
        while(currentNode != _startNode);

        Node* nextNode = nearestNode->next;
        nearestNode->next = new Node(p);
        nearestNode->next->next = nextNode;
    }
    else
    {
        _startNode = new Node(p);
        _startNode->next = _startNode;
    }
}
