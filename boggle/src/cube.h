#ifndef CUBE_H
#define CUBE_H

#include <string>

using namespace std;

class Cube
{
public:
    Cube(const string& sides);
    Cube();

    /**
     * Changes the current side of the cube.
     */
    void roll();

    /**
     * Returns the character of the current side.
     * @return The character of the current side
     */
    char getCurrentSide();

private:
    string sides;
    int currentSide;
};

#endif // CUBE_H
