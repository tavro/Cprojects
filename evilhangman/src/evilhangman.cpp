/**
 * Opens a game of hangman against a cheating opponent
 * @file evilhangman.cpp
 * @authors vikho305 & isaho220
 */

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/**
 * Creates a set of all lines from the file dictionary.txt
 * @return A set of all words
 */
set<string> getDictionary()
{
    set<string> dictionary;
    ifstream dictFile("dictionary.txt");

    if (dictFile.is_open())
    {
        string dictWord;
        while(getline(dictFile, dictWord))
        {
            dictionary.insert(dictWord);
        }

        dictFile.close();
    }

    return dictionary;
}

/**
 * Stores all words of a specific length in set
 * @param words - The set to store the words
 * @param dictionary - The dictionary to get the words from
 * @param length - The length of the words to get
 */
void getWordsOfLength(set<string>& words, const set<string>& dictionary, int length)
{
    words.clear();

    for(string word : dictionary)
    {
        if (word.length() == length) words.insert(word);
    }
}

/**
 * Let's the user input a yes/no answer
 * @param prompt - The essage to print before getting input
 * @return Whether you've answered "y" or not
 */
bool cinYesOrNo(string prompt)
{
    string answer = "";
    while(answer != "y" && answer != "n")
    {
        cout << prompt << " (y/n) ";
        cin >> answer;
    }

    return answer == "y";
}

/**
 * Let's the user input a number
 * @param prompt - The essage to print before getting input
 * @return The number that was inputted
 */
int cinNumber(string prompt)
{
    int answer = 0;
    cout << prompt << " ";

    while(true) // Runs until the input is a valid integer
    {
        if (cin >> answer)
            return answer;
        else
            cout << "Try again! ";

        cin.clear(); // Clears error flag so that the if case works
        cin.ignore(10000, '\n'); // Ignores up to 10000 incorrect inputs (if answer is not a number each char counts as one cin input)
    }
}

/**
 * Let's the user input a single character
 * @param prompt - The essage to print before getting input
 * @return The letter that was inputted
 */
char cinLetter(string prompt)
{
    string letter = "";
    cout << prompt << " ";

    while(true) // Runs until the input is a valid char
    {
        cin >> letter;

        if (letter.length() == 1)
        {
            for(char l : ALPHABET)
            {
                if(letter[0] == l) return letter[0];
            }
        }

        cout << "Try again! ";
    }
}

/**
 * Prints the UI of a game of hangman
 * @param guessesLeft - How many guesses the player has left
 * @param guessedLetters - The letters the player has guessed already
 * @param guessedWord - The currently revealed letters
 */
void printGameStatus(int guessesLeft, const set<char>& guessedLetters, string guessedWord)
{
    cout << "Guesses left: " << guessesLeft << endl;
    cout << "Guessed letters: ";
    for(char letter : guessedLetters)
    {
        cout << letter << ", ";
    }
    cout << endl;
    cout << "Word: " << guessedWord << endl;
}

/**
 * Prints the UI of a game of hangman
 * @param guessesLeft - How many guesses the player has left
 * @param guessedLetters - The letters the player has guessed already
 * @param possibleWords - The words could end up being the correct one
 * @param guessedWord - The currently revealed letters
 */
void printGameStatus(int guessesLeft, const set<char>& guessedLetters, string guessedWord, const set<string>& possibleWords)
{
    cout << "Possible words: ";
    for (string possibleWord : possibleWords)
    {
        cout << possibleWord << ", ";
    }
    cout << endl;

    printGameStatus(guessesLeft, guessedLetters, guessedWord);
}

/**
 * Removes the smallest group of word families from a set based on a letter
 * @param words - The words to group into word families
 * @param letter - The letter to base the word families on
 */
void removeSmallWordFamilies(set<string>& words, char letter, string& guessedWord)
{
    map<string, set<string>> wordFamilies;
    set<string> biggestWordFamily;

    string s = "";
    // Group words together based on if they have the letter in the same spots
    for(string word : words)
    {
        string familyKey = ""; // E.g. 0101, 0000, 1011
        for(char wordLetter : word)
        {
            if (wordLetter == letter) familyKey += "1";
            else familyKey += "0";
        }
        wordFamilies[familyKey].insert(word);
    }

    // Select the biggest word family
    string biggestWordFamilyKey;
    for(map<string, set<string>>::iterator it = wordFamilies.begin(); it != wordFamilies.end(); it++)
    {
        if (it->second.size() > biggestWordFamily.size())
        {
            biggestWordFamily = it->second;
            biggestWordFamilyKey = it->first;
        }
    }

    for(int i = 0; i < biggestWordFamilyKey.length(); i++)
    {
        if(biggestWordFamilyKey[i] == '1')
        {
            guessedWord[i] = letter;
        }
    }

    words = biggestWordFamily;
}

int main()
{
    cout << "Welcome to Hangman." << endl;

    set<string> dictionary = getDictionary();
    set<string> wordsOfLength;

    bool running = true;
    while(running)
    {
        string guessedWord = "";
        cout << endl;

        int wordLength;
        bool lookingForWordLength = true;
        while(lookingForWordLength) // Runs until it has found a word of length wordLength
        {
            wordLength = cinNumber("How long will the word be?");
            for(string word : dictionary)
            {
                if (word.length() == wordLength)
                {
                    lookingForWordLength = false;
                    break; // Stop looking for words of length wordLength
                }
            }
        }

        for(int i = 0; i < wordLength; i++)
        {
            guessedWord += "_";
        }

        int guessCount = cinNumber("How many guesses do you want?");
        bool showPossibleWords = cinYesOrNo("Do you want to see possible words?");
        cout << endl;

        getWordsOfLength(wordsOfLength, dictionary, wordLength);
        set<char> guessedLetters;

        for(int guess = 0; guess < guessCount; guess++)
        {
            if (showPossibleWords)
                printGameStatus(guessCount - guess, guessedLetters, guessedWord, wordsOfLength);
            else
                printGameStatus(guessCount - guess, guessedLetters, guessedWord);

            char letter = cinLetter("Which letter do you want to guess?");
            while (guessedLetters.find(letter) != guessedLetters.end())
                letter = cinLetter("Already guessed!");
            guessedLetters.insert(letter);

            removeSmallWordFamilies(wordsOfLength, letter, guessedWord);

            // Check if you have guessed all the letters of the only possible word
            if (wordsOfLength.size() == 1)
            {
                string word = *wordsOfLength.begin();

                bool wordHasBeenGuessed = true;
                for (char letter : word) {
                    if (guessedLetters.find(letter) == guessedLetters.end()) wordHasBeenGuessed = false;
                }

                if (wordHasBeenGuessed) {
                    cout << "\nThe word is " << word << "!" << endl;
                    cout << "You win!" << endl;
                    guess = guessCount; // Exits the guess loop
                }
            }

            cout << endl;
        }

        running = cinYesOrNo("Do you want to play again?");
    }
}
