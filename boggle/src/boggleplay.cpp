/**
 * Defines how one game of Boggle is played
 * @file boggleplay.cpp
 * @authors vikho305 & isaho220
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    if (yesOrNo("Do you want to generate a random board? ")) {
        boggle.shuffleBoard();
    }
    else {
        string configuration = "";
        bool configuring = true;
        while (configuring)
        {
            cout << "Type the 16 letters to appear on the board: ";
            getline(cin, configuration);

            // Validate configuration
            if (configuration.length() == 16)
            {
                bool isAlpha = true;
                string::iterator it = configuration.begin();
                while (isAlpha && it != configuration.end())
                {
                    if (isalpha(*it) == 0)
                        isAlpha = false;
                    it++;
                }

                if (isAlpha)
                    configuring = false;
                else
                    cout << "That is not an alphabetical word! Try again." << endl;
            }
            else
            {
                cout << "That is not a 16-letter board String! Try again." << endl;
            }
        }
        configuration = toUpperCase(configuration);

        // Apply configuration
        boggle.configureBoard(configuration);
    }
    boggle.init();

    // Player turn
    cout << "It's your turn!" << endl;

    bool isPlayerTurn = true;
    while (isPlayerTurn) {
        cout << boggle.getBoardString() << boggle.getPlayerUI() << endl;

        cout << "Type a word (or press Enter to end your turn): ";
        string input;
        getline(cin, input);
        input = toUpperCase(input);

        if (input.length() == 0)
            isPlayerTurn = false;
        else if (boggle.validateWord(input))
            cout << "You found a new word! \"" << input << "\"" << endl;
        else
            cout << "You must guess a valid word!" << endl;
    }

    // Computer turn
    cout << endl;
    cout << "It's my turn!" << endl;

    boggle.findComputerWords();
    cout << boggle.getComputerUI();

    if (boggle.getWinStatus() < 0)
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    else if (boggle.getWinStatus() > 0)
        cout << "Oh no! How could a pathetic, tiny human have destroyed me?!" << endl;
    else
        cout << "There must be a bug or something. I demand a rematch!" << endl;
}

/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
