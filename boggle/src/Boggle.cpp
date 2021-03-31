/**
 * Defines all the functionality needed for a game of Boggle
 * @file Boggle.cpp
 * @authors vikho305 & isaho220
 */

#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

string setToString(const unordered_set<string>& toPrint)
{
    string tmp = "{";

    unordered_set<string>::const_iterator it = toPrint.cbegin();
    for (int i = 0; i < toPrint.size(); i++, it++)
    {
        tmp += *it;
        if (i != toPrint.size() - 1)
            tmp += ", ";
    }

    tmp += "}";
    return tmp;
}

// Public members below

Boggle::Boggle()
{
    lexicon = Lexicon(DICTIONARY_FILE);

    // Init board
    board = new Cube*[BOARD_SIZE];
    for(int x = 0; x < BOARD_SIZE; x++)
        board[x] = new Cube[BOARD_SIZE];
}

Boggle::~Boggle()
{
    for(int x = 0; x < BOARD_SIZE; x++)
        delete[] board[x];
    delete[] board;
}

void Boggle::shuffleBoard()
{
    for(int x = 0, c = 0; x < BOARD_SIZE; x++)
    {
        for(int y = 0; y < BOARD_SIZE; y++, c++)
        {
            Cube cube(CUBES[c]);
            board[x][y] = cube;
            board[x][y].roll();
        }
    }
    shuffle(board, BOARD_SIZE, BOARD_SIZE);
}

void Boggle::configureBoard(const string& configuration)
{
    for(int x = 0, i = 0; x < BOARD_SIZE; x++)
    {
        for(int y = 0; y < BOARD_SIZE; y++, i++)
        {
            Cube cube(string(6, configuration[i]));
            board[x][y] = cube;
        }
    }
}

void Boggle::init()
{
    playerPoints = 0;
    playerWords.clear();
    computerPoints = 0;
    computerWords.clear();
}

string Boggle::getBoardString()
{
    string result = "";

    for(int y = 0; y < BOARD_SIZE; y++)
    {
        for(int x = 0; x < BOARD_SIZE; x++)
        {
            result += board[x][y].getCurrentSide();
        }
        result += "\n";
    }

    return result;
}

string Boggle::getPlayerUI()
{
    string result = "";

    result += "Your words (" + to_string(playerWords.size()) + "): " + setToString(playerWords) + "\n";
    result += "Your score: " + to_string(playerPoints) + "\n";

    return result;
}

string Boggle::getComputerUI()
{
    string result = "";

    result += "My words (" + to_string(computerWords.size()) + "): " + setToString(computerWords) + '\n';
    result += "My score: " + to_string(computerPoints) + "\n";

    return result;
}

void Boggle::findComputerWords()
{
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            unordered_set<Cube*> traversed;
            findComputerWordsHelper(x, y, "", traversed);
        }
    }
}

bool Boggle::validateWord(const string &word)
{
    for (int x = 0; x < BOARD_SIZE; x++)
    {
        for (int y = 0; y < BOARD_SIZE; y++)
        {
            unordered_set<Cube*> traversed;
            if (validateWordHelper(x, y, "", word, traversed)) {
                playerPoints += word.length() - 3;
                playerWords.insert(word);
                return true;
            }
        }
    }
    return false;
}

int Boggle::getWinStatus()
{
    return playerPoints - computerPoints;
}

// Private members below

void Boggle::findComputerWordsHelper(int x, int y, string chosen, unordered_set<Cube *> traversed)
{
    Cube& cube = board[x][y];
    chosen += cube.getCurrentSide();

    if (traversed.find(&cube) == traversed.end() && lexicon.containsPrefix(chosen))
    {
        traversed.insert(&cube);

        if (playerWords.find(chosen) == playerWords.end() && lexicon.contains(chosen) && chosen.length() >= 4) {
            computerPoints += chosen.length() - 3;
            computerWords.insert(chosen);
        }

        for (int newX = x - 1; newX <= x + 1; newX++)
        {
            for (int newY = y - 1; newY <= y + 1; newY++)
            {
                bool isInBounds = newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE;
                if (isInBounds && (newX != x || newY != y) )
                    findComputerWordsHelper(newX, newY, chosen, traversed);
            }
        }
    }
}

bool Boggle::validateWordHelper(int x, int y, string chosen, const string &word, unordered_set<Cube *> traversed)
{
    Cube& cube = board[x][y];
    chosen += cube.getCurrentSide();

    if (traversed.find(&cube) == traversed.end() && lexicon.containsPrefix(chosen) && word.substr(0, chosen.length()) == chosen)
    {
        traversed.insert(&cube);

        if (playerWords.find(chosen) == playerWords.end() && lexicon.contains(chosen) && chosen.length() >= 4)
            return true;

        for (int newX = x - 1; newX <= x + 1; newX++)
        {
            for (int newY = y - 1; newY <= y + 1; newY++)
            {
                bool isInBounds = newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE;
                if (isInBounds && (newX != x || newY != y) && validateWordHelper(newX, newY, chosen, word, traversed))
                    return true;
            }
        }
    }

    return false;
}
