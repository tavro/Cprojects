/**
 * Declares all the functionality needed for a game of Boggle
 * @file Boggle.h
 * @authors vikho305 & isaho220
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>

// Student-inclusions below
#include <unordered_set>
#include "cube.h"
#include "lexicon.h"

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    // Student-declared members below
    Boggle();
    ~Boggle();

    /**
     * Resets and shuffles the cubes on the board
     */
    void shuffleBoard();

    /**
     * Configures the letters on the board
     */
    void configureBoard(const string& configuration);

    /**
     * Initializes the game
     */
    void init();

    /**
     * Prints the board to the console
     */
    string getBoardString();

    /**
     * Prints all player data to the console
     */
    string getPlayerUI();

    /**
     * Prints all computer player data to the console
     */
    string getComputerUI();

    /**
     * Finds all the remaining words that the player hasn't found
     */
    void findComputerWords();

    /**
     * Checks whether or not a string can be found on the board.
     * @param target - The string to find
     * @return Whether or not target has been found
     */
    bool validateWord(const string& word);

    /**
     * Checks whether the player or computer has won
     * @return >0 if the player has won, <0 if the computer has won
     */
    int getWinStatus();

private:
    Lexicon lexicon;
    Cube** board;
    int playerPoints;
    unordered_set<string> playerWords;
    int computerPoints;
    unordered_set<string> computerWords;

    /**
     * Finds all the remaining words that the player hasn't found from an origin point
     * @param x - The horizontal origin point
     * @param y - The vertical origin point
     * @param chosen - The current found potential word
     * @param traversed - The set to store the already traversed cubes
     */
    void findComputerWordsHelper(int x, int y, string chosen, unordered_set<Cube*> traversed);

    /**
     * Checks whether or not a string can be found on the board from a specific origin point.
     * @param x - The horizontal origin point
     * @param y - The vertical origin point
     * @param chosen - The current found potential word
     * @param target - The string to find
     * @param traversed - The set to store the already traversed cubes
     * @return Whether or not target has been found
     */
    bool validateWordHelper(int x, int y, string chosen, const string& word, unordered_set<Cube*> traversed);
};

#endif
