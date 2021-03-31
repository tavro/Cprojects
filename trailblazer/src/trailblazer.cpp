/**
 * Defines all methods of search for a graph, from start to finish
 * @file trailblazer.cpp
 * @authors vikho305 & isaho220
 */

#include "costs.h"
#include "trailblazer.h"
#include <stack>
#include <queue>

#include "pqueue.h"

using namespace std;

/**
 * Find a path from one vertex to another via the dfs algorithm
 * @param graph The graph to search on
 * @param start The vertex to find the path from
 * @param end The vertex to find the path to
 * @return A vertex vector representing the path
 */
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;

    // Reset all edges & vertices
    graph.resetData();

    // Find end vertex from start vertex
    stack<Vertex*> vertexStack;
    vertexStack.push(start);
    start->setColor(YELLOW);

    bool endIsFound = false;
    while(!vertexStack.empty() && !endIsFound) {
        Vertex* current = vertexStack.top();
        vertexStack.pop();

        // Visit each non-visited neighbor
        bool atDeadEnd = true;
        for(Edge* edge : current->arcs) {
            Vertex* next = edge->finish;

            if (!next->visited) {
                next->previous = current;
                atDeadEnd = false;

                if (next == end) {
                    endIsFound = true; // Stops the searching
                    next->setColor(GREEN);
                }
                else {
                    vertexStack.push(next);
                    next->setColor(YELLOW);
                }
            }
        }

        current->visited = true;
        if (atDeadEnd)
            current->setColor(GRAY);
        else
            current->setColor(GREEN);
    }

    // Add vertices to path vector
    Vertex* current = end;
    while(current != nullptr) {
        path.insert(path.begin(), current);
        current = current->previous;
    }

    return path;
}

/**
 * Find a path from one vertex to another via the bfs algorithm
 * @param graph The graph to search on
 * @param start The vertex to find the path from
 * @param end The vertex to find the path to
 * @return A vertex vector representing the path
 */
vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;

    // Reset all edges & vertices
    graph.resetData();

    // Find end vertex from start vertex
    queue<Vertex*> vertexQueue;
    vertexQueue.push(start);
    start->setColor(YELLOW);

    bool endIsFound = false;
    while(!vertexQueue.empty() && !endIsFound) {
        Vertex* current = vertexQueue.front();
        vertexQueue.pop();

        // Visit each non-visited neighbor
        for(Edge* edge : current->arcs) {
            Vertex* next = edge->finish;

            if (!next->visited) {
                next->previous = current;

                if (next == end) {
                    endIsFound = true; // Stops the searching
                    next->setColor(GREEN);
                }
                else {
                    vertexQueue.push(next);
                    next->setColor(YELLOW);
                }
            }
        }

        current->visited = true;
        current->setColor(GREEN);
    }

    // Add vertices to path vector
    Vertex* current = end;
    while(current != nullptr) {
        path.insert(path.begin(), current);
        current = current->previous;
    }

    return path;
}

/**
 * Find a path from one vertex to another via the dijkstras algorithm
 * @param graph The graph to search on
 * @param start The vertex to find the path from
 * @param end The vertex to find the path to
 * @return A vertex vector representing the path
 */
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;

    // Reset all edges & vertices
    graph.resetData();
    for(Vertex* vertex : graph.getVertexSet())
        vertex->cost = INFINITY;

    // Find end vertex from start vertex
    PriorityQueue<Vertex*> vertexQueue;
    vertexQueue.enqueue(start, 0);
    start->setColor(YELLOW);
    start->cost = 0;

    bool endIsFound = false;
    while(vertexQueue.size() > 0 && !endIsFound) {
        Vertex* current = vertexQueue.dequeue();

        // Visit each neighbor
        for(Edge* edge : current->arcs) {
            Vertex* next = edge->finish;

            // Only interact with neighbor if its current cost is greater than its potential cost
            if (next->cost > current->cost + edge->cost)  {
                next->cost = current->cost + edge->cost;
                next->previous = current;

                if (next == end) {
                    endIsFound = true; // Stops the searching
                    next->setColor(GREEN);
                }
                else if (!next->visited) {
                    vertexQueue.enqueue(next, next->cost);
                    next->setColor(YELLOW);
                    next->visited = true; // Marks as visited immediately to avoid enqueuing the same vertex twice (AKA enter this else-if statement)
                }
                else {
                    vertexQueue.changePriority(next, next->cost);
                    next->setColor(YELLOW);
                }
            }
        }

        current->visited = true;
        current->setColor(GREEN);
    }

    // Add vertices to path vector
    Vertex* current = end;
    while(current != nullptr) {
        path.insert(path.begin(), current);
        current = current->previous;
    }

    return path;
}

/**
 * Find a path from one vertex to another via the a* algorithm
 * @param graph The graph to search on
 * @param start The vertex to find the path from
 * @param end The vertex to find the path to
 * @return A vertex vector representing the path
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;

    // Reset all edges & vertices
    graph.resetData();
    for(Vertex* vertex : graph.getVertexSet())
        vertex->cost = INFINITY;

    // Find end vertex from start vertex
    PriorityQueue<Vertex*> vertexQueue;
    vertexQueue.enqueue(start, 0);
    start->setColor(YELLOW);
    start->cost = 0;

    bool endIsFound = false;
    while(vertexQueue.size() > 0 && !endIsFound) {
        Vertex* current = vertexQueue.dequeue();

        // Visit each neighbor
        for(Edge* edge : current->arcs) {
            Vertex* next = edge->finish;

            // Only interact with neighbor if its current cost is greater than its potential cost
            if (next->cost > current->cost + edge->cost)  {
                next->cost = current->cost + edge->cost;
                next->previous = current;

                if (next == end) {
                    endIsFound = true; // Stops the searching
                    next->setColor(GREEN);
                }
                else if (!next->visited) {
                    vertexQueue.enqueue(next, next->cost + next->heuristic(end)); // Priority is potential cost of path including this vertex
                    next->setColor(YELLOW);
                    next->visited = true; // Marks as visited immediately to avoid enqueuing the same vertex twice (AKA enter this else-if statement)
                }
                else {
                    vertexQueue.changePriority(next, next->cost + next->heuristic(end)); // Priority is potential cost of path including this vertex
                    next->setColor(YELLOW);
                }
            }
        }

        current->visited = true;
        current->setColor(GREEN);
    }

    // Add vertices to path vector
    Vertex* current = end;
    while(current != nullptr) {
        path.insert(path.begin(), current);
        current = current->previous;
    }

    return path;
}
