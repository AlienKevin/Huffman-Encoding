#include "encoding.h"
#include "priorityqueue.h"

void buildEncodingMapHelper(HuffmanNode* encodingTree, string encoding, Map<char, string>& encodingMap);

Map<char, int> buildFrequencyTable(istream& input) {
    Map<char, int> table = Map<char, int>();
    char ch;
    while (input.get(ch)) {
        table[ch] ++;
    }
    return table;
}

HuffmanNode* buildEncodingTree(Map<char, int>& freqTable) {
    PriorityQueue<HuffmanNode*> forest = PriorityQueue<HuffmanNode*>();
    // load all characters (singleton trees) into forest
    for (int i = 0; i < freqTable.keys().size(); i++) {
        char ch = freqTable.keys()[i];
        int weight = freqTable[ch];
        // create a new HuffmanNode for each character
        HuffmanNode* newNode = new HuffmanNode(ch);
        // put the new node into the forest
        forest.enqueue(newNode, weight);
    }
    // do the building
    HuffmanNode* root = nullptr;
    while (forest.size() > 1) { // not all subtrees have been combined under one single root
        // find and remove two least weighted subtrees from the forest
        int node1Priority = forest.peekPriority();
        HuffmanNode* node1 = forest.dequeue();
        int node2Priority = forest.peekPriority();
        HuffmanNode* node2 = forest.dequeue();
        // combine these two subtrees under one single root
        root = new HuffmanNode(node1, node2);
        int rootPriority = node1Priority + node2Priority;
        // put the root back to the forest
        forest.enqueue(root, rootPriority);
    }
    return root;
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

Map<char, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<char, string> map = Map<char, string>();
    buildEncodingMapHelper(encodingTree, "", map);
    return map;
}

void buildEncodingMapHelper(HuffmanNode* encodingTree, string encoding, Map<char, string>& encodingMap) {
    if (encodingTree->isLeaf()) {
        encodingMap[encodingTree->ch] = encoding;
    } else {
        buildEncodingMapHelper(encodingTree->zero, encoding + "0", encodingMap);
        buildEncodingMapHelper(encodingTree->one, encoding + "1", encodingMap);
    }
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
