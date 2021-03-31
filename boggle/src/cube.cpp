#include "cube.h"
#include "random.h"

Cube::Cube(const string& sides){
    this->sides = sides;
    currentSide = 0;
}

Cube::Cube(){
    sides = "ABCDEF";
    currentSide = 0;
}

void Cube::roll(){
    currentSide = randomInteger(0, sides.length() - 1);
}

char Cube::getCurrentSide(){
    return sides[currentSide];
}
