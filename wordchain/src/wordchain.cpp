/**
 * Transforms one word into another, one letter at a time
 * @file wordchain.cpp
 * @authors vikho305 & isaho220
 */

#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>

#include <set>
using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";

/**
 * Initializes the set validWords with valid english words
 */
void initValidWords(set<string>& validWords) {
    ifstream wordFile("dictionary.txt");

    if (wordFile.is_open()) {
        string word;
        while (getline(wordFile, word)) {
            validWords.insert(word);
        }
        wordFile.close();
    }
}

/**
 * Prints a given word chain to the console
 * @param wordChain - The word chain to print
 */
void printWordChain(stack<string>& wordChain) {
    while (!wordChain.empty()) {
        string word = wordChain.top();
        wordChain.pop();

        if (wordChain.empty())
            cout << word << endl;
        else
            cout << word << " <- ";
    }
}

/**
 * Gets the neighbors of a word
 * @param neighbors - The set to store the neighbors
 * @param word - The word from which to get neighbors
 */
void getValidNeighbors(set<string>& neighbors, string word, set<string>& validWords) {
    neighbors.clear();

    for (char& letter : word) {
        char originalLetter = letter; // Store to original letter to preserve to original word
        for (char newLetter : ALPHABET) {
            if (newLetter != originalLetter) { // If the new word equals the original word, skip
                letter = newLetter;

                if (validWords.find(word) != validWords.end()) // If new word is valid, add
                    neighbors.insert(word);
            }
        }
        letter = originalLetter; // Restore the original word
    }
}

/**
 * Generates and prints the short possible word chain, from one word to another
 * @param w1 - The word to transform
 * @param w2 - The word to transform w1 into
 */
void wordChain(string w1, string w2, set<string>& validWords) {
    queue<stack<string>> wordChainQueue;
    stack<string> firstWordChain;
    firstWordChain.push(w1);
    wordChainQueue.push(firstWordChain);
    set<string> validNeighbors;
    set<string> wordsInChains; // Stores all words that have been used in a chain
    wordsInChains.insert(w1);

    while (!wordChainQueue.empty()) {
        stack<string> currentWordChain = wordChainQueue.front();
        wordChainQueue.pop();

        if (currentWordChain.top() == w2) {
            cout << "Words checked: " << wordsInChains.size() << endl;
            printWordChain(currentWordChain); // Print the found valid word chain
            return; // Stop checking for word chains
        }
        else {
            string currentWord = currentWordChain.top();
            getValidNeighbors(validNeighbors, currentWord, validWords);

            for (string neighbor : validNeighbors) {
                if (wordsInChains.find(neighbor) == wordsInChains.end()) {
                    wordsInChains.insert(neighbor);
                    stack<string> copiedWordChain = currentWordChain;
                    copiedWordChain.push(neighbor);
                    wordChainQueue.push(copiedWordChain);
                }
            }
        }
    }

    cout << "Tranformation failed!" << endl;
}

int main() {
    set<string> validWords;
    initValidWords(validWords);

    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    cout << "Please type two words: ";
    string firstWord;
    string secondWord;
    cin >> firstWord >> secondWord;

    wordChain(firstWord, secondWord, validWords);

    return 0;
}
