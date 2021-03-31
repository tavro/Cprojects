/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"

GameState::GameState(){}

GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {
        Robot* robot = new Robot();
        while(!isEmpty(*robot)) {
            robot->teleport();
        }
        robots.push_back(robot);
    }
    teleportHero();
}

GameState::GameState(const GameState& gs){
    Hero heroCopy(gs.hero);
    hero = heroCopy;

    for(Robot* robot : gs.robots){
        robot->teleport();
    }
}

GameState& GameState::operator=(const GameState& gs){
    if (gs != *this) {
        Hero heroCopy(gs.hero);
        hero = heroCopy;

        for(Robot* robot : robots) {
            delete robot;
            robots.pop_back();
        }

        for(Robot* robot : gs.robots){
            Robot* robotClone = robot->copy();
            robots.push_back(robotClone);
        }
    }

    return *this;
}

GameState::~GameState(){
    for(Robot* robot : robots) {
        delete robot;
    }
}

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robots.size(); ++i)
        robots[i]->draw(scene);
    hero.draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++)
        robots[i]->moveTowards(hero);
}

int GameState::countCollisions() {
    int numberDestroyed = 0;

    /*
    for (Robot* robot : robots) // For debug
    {
        std::cout << (dynamic_cast<Junk*>(robot) != nullptr) << std::endl;
    }
    */

    unsigned int i = 0;
    while (i < robots.size())
    {
        Robot* robot = robots[i];

        bool hitJunk = junkAt(*robot);
        bool collision = countRobotsAt(*robot) > 1;
        if (dynamic_cast<Junk*>(robot) == nullptr && (hitJunk || collision))
        {
            if (!hitJunk)
            {
                Junk* newJunk = new Junk(*robot);
                //std::cout << "Created junk " << newJunk << std::endl; // For debug
                robots.push_back(newJunk);
            }
            //std::cout << "Deleted " << robot << std::endl; // For debug
            delete robot;
            //std::cout << "Replaced " << robots[i] << " with " << robots[robots.size() - 1] << std::endl; // For debug
            robots[i] = robots[robots.size() - 1];
            robots.pop_back();
            numberDestroyed++;
        }
        else i++;
    }

    //std::cout << std::endl; // For debug

    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    for (Robot* robot : robots) {
        if (dynamic_cast<Junk*>(robot) == nullptr)
            return true;
    }
    return false;
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++)
        if (robots[i]->attacks(unit)) return false;
    if (junkAt(unit)) return false;
    return true;
}

void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    return (countRobotsAt(unit) == 0 && !junkAt(unit));
}

/*
 * Is there junk at unit?
 */
bool GameState::junkAt(const Unit& unit) const {
    for (size_t i = 0; i < robots.size(); ++i) {
        if (dynamic_cast<Junk*>(robots[i]) != nullptr && robots[i]->at(unit))
            return true;
    }
    /*
    for (size_t i = 0; i < junks.size(); ++i)
        if (junks[i].at(unit)) return true;
    */
    return false;
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        if (dynamic_cast<Junk*>(robots[i]) == nullptr && robots[i]->at(unit))
            count++;
    }
    return count;
}
