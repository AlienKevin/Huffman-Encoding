// TODO: remove and replace this file header comment
// This is the CPP file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.

#include "encoding.h"
#include "priorityqueue.h"

Map<char, int> buildFrequencyTable(istream& input)
{
    // A line that reads: (void)parameter 
    // is included in starter to suppress warnings about
    // unused parameters for as-yet-unimplemented functions
    // Remove these lines when ready to implement the function

    // TODO: implement this function
    (void) input;
    return {};
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
