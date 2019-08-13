#include "encoding.h"
#include "priorityqueue.h"

Map<char, int> buildFrequencyTable(istream& input)
{
    Map<char, int> table = Map<char, int>();
    char ch;
    while (input.get(ch)) {
        table[ch] ++;
    }
    return table;
}

HuffmanNode* buildEncodingTree(Map<char, int>& freqTable)
{
    // TODO: implement this function
    (void) freqTable;
    return nullptr;
}

string flattenTreeToHeader(HuffmanNode* t)
{
    // TODO: implement this function
    (void) t;
    return "";
}

HuffmanNode* recreateTreeFromHeader(string str)
{
    // TODO: implement this function
    (void) str;
    return nullptr;
}

Map<char, string> buildEncodingMap(HuffmanNode* encodingTree)
{
    // TODO: implement this function
    (void) encodingTree;
    return {};
}

void freeTree(HuffmanNode* t)
{
    // TODO: implement this function
    (void) t;
}

void compress(istream& input, HuffmanOutputFile& output)
{
    // TODO: implement this function
    (void) input;
    (void) output;
}

void decompress(HuffmanInputFile& input, ostream& output)
{
    // TODO: implement this function
    (void) input;
    (void) output;
}
