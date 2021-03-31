/**
 * Defines all the steps of encoding/decoding & compressing/decompressing data
 * @file encoding.cpp
 * @authors vikho305 & isaho220
 */

#include "encoding.h"

#include <queue>

map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;

    do {
        int character = input.get();

        if (input.gcount() != 0)
            freqTable[character]++;
        else
            freqTable[PSEUDO_EOF]++;
    }
    while(input.gcount() != 0);

    return freqTable;
}

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode> priorityQueue;

    // Fill priority queue
    map<int, int>::const_iterator freqIt = freqTable.cbegin();
    while (freqIt != freqTable.cend()) {
        HuffmanNode node(freqIt->first, freqIt->second);
        priorityQueue.push(node);
        freqIt++;
    }

    // Combine nodes
    while (priorityQueue.size() > 1) {
        HuffmanNode* zeroNode = new HuffmanNode(priorityQueue.top());
        priorityQueue.pop();
        HuffmanNode* oneNode = new HuffmanNode(priorityQueue.top());
        priorityQueue.pop();

        HuffmanNode parentNode(NOT_A_CHAR, zeroNode->count + oneNode->count, zeroNode, oneNode);
        priorityQueue.push(parentNode);
    }

    return new HuffmanNode(priorityQueue.top());
}

void fillEncodingMap(map<int, string>& encodingMap, HuffmanNode* encodingNode, string bitSeq="") {
    if (encodingNode->character != NOT_A_CHAR) {
        encodingMap[encodingNode->character] = bitSeq;
        return; // Prevents calling the method with null encoding nodes
    }

    if (encodingNode->zero != nullptr)
        fillEncodingMap(encodingMap, encodingNode->zero, bitSeq + '0');

    if (encodingNode->one != nullptr)
        fillEncodingMap(encodingMap, encodingNode->one, bitSeq + '1');
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;
    fillEncodingMap(encodingMap, encodingTree);
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    do {
        int character = input.get();

        string bitSeq;
        if (character != EOF)
            bitSeq = encodingMap.at(character);
        else
            bitSeq = encodingMap.at(PSEUDO_EOF);

        for (char bit : bitSeq) {
            if (bit == '0')
                output.writeBit(0);
            else
                output.writeBit(1);
        }
    }
    while(input.gcount() != 0);
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* currentNode = encodingTree;

    bool decoding = true;
    while (decoding) {
        int bit = input.readBit();

        // Traverse nodes via bit values...
        if (bit == 0)
            currentNode = currentNode->zero;
        else
            currentNode = currentNode->one;

        // ...until a character is found
        if (currentNode->character == PSEUDO_EOF) {
            // output.put(EOF);
            decoding = false;
        }
        else if (currentNode->character != NOT_A_CHAR) {
            output.put(currentNode->character);
            currentNode = encodingTree;
        }
    }
}

/*
 * Converts a frequency table to a frequency string, then writes it to the output
 */
void writeFrequencyString(const map<int, int>& freqTable, obitstream& output) {
    string freqString = "{";

    map<int, int>::const_iterator freqIt = freqTable.cbegin();
    while (freqIt != freqTable.cend()) {
        freqString += to_string(freqIt->first);
        freqString += ':';
        freqString += to_string(freqIt->second);
        freqString += ',';

        freqIt++;
    }
    freqString.erase(freqString.length() - 1, 1); // Remove unecessary comma

    output << freqString + '}';
}

void compress(istream& input, obitstream& output) {
    map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    map<int, string> encodingMap = buildEncodingMap(encodingTree);

    writeFrequencyString(freqTable, output);
    input.clear();
    input.seekg(0, ios::beg);
    encodeData(input, encodingMap, output);

    freeTree(encodingTree);
}

/*
 * Converts a frequency string to a frequency table
 */
map<int, int> readFrequencyString(const string& freqString) {
    map<int, int> freqTable;

    string::const_iterator freqIt = freqString.cbegin() + 1;
    string::const_iterator lastSplitIt = freqIt;
    while (freqIt != freqString.cend()) {
        if (*freqIt == ',' || *freqIt == '}') {
            string keyValuePair = string(lastSplitIt, freqIt);
            lastSplitIt = freqIt + 1;

            int splitIndex = keyValuePair.find(':');
            string keyStr = keyValuePair.substr(0, splitIndex);
            string valueStr = keyValuePair.substr(splitIndex + 1, keyValuePair.length() - 1);
            int key = 0;
            int value = 0;

            stringstream keyStream(keyStr);
            stringstream valueStream(valueStr);
            keyStream >> key;
            valueStream >> value;

            freqTable[key] = value;
        }

        freqIt++;
    }

    return freqTable;
}

void decompress(ibitstream& input, ostream& output) {
    string freqString = "";

    // Read frequency string from input stream
    bool convertingFreqString = true;
    while (convertingFreqString) {
        int inputByte = input.get();
        freqString += inputByte;

        if (inputByte == '}')
            convertingFreqString = false;
    }

    map<int, int> freqTable = readFrequencyString(freqString);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

void freeTree(HuffmanNode* node) {
    if (node->zero != nullptr)
        freeTree(node->zero);
    if (node->one != nullptr)
        freeTree(node->one);
    delete node;
}
