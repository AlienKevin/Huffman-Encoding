#include "encoding.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "stack.h"
#include "filelib.h"

void buildEncodingMapHelper(HuffmanNode* encodingTree, string encoding, Map<char, string>& encodingMap);
void recreateTreeFromHeaderHelper(string str, HuffmanNode* tree);

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

string flattenTreeToHeader(HuffmanNode* t) {
    // do a pre-order traversal of the tree to generate header
    if (t->isLeaf()) {
        // write a leaf node in the form .A (i.e. a period followed by the specific character)
        return "." + charToString(t->ch);
    } else {
        // write an interior node as a pair of parens wrapped around its two child nodes (ZO)
        // (Z and O could each be either leaf or internal nodes)
        return "(" + flattenTreeToHeader(t->zero) + flattenTreeToHeader(t->one) + ")";
    }
}

HuffmanNode* recreateTreeFromHeader(string str) {
    HuffmanNode* tree = new HuffmanNode(nullptr, nullptr);
    recreateTreeFromHeaderHelper(str.substr(1, str.size() - 2), tree);
    return tree;
}

int findMatchingParenthesis(string str) {
    Stack<bool> parenStack = Stack<bool>();
    parenStack.push(true);
    int currentIndex = 1;
    while (currentIndex < str.size() && !parenStack.isEmpty()) {
        if (str[currentIndex] == '(') {
            parenStack.push(true);
        } else if (str[currentIndex] == ')') {
            parenStack.pop();
        }
        currentIndex ++;
    }
    if (parenStack.isEmpty()) {
        return currentIndex - 1;
    }
    return -1;
}

void recreateTreeFromHeaderHelper(string str, HuffmanNode* tree) {
    if (str.size() == 0) {
        return;
    }
    cout << "str: " << str << endl;
    if (str[0] == '(') {
        int closeParenIndex = findMatchingParenthesis(str);
        cout << "closeParenIndex: " << closeParenIndex << endl;
        tree->zero = new HuffmanNode(nullptr, nullptr);
        recreateTreeFromHeaderHelper(str.substr(1, closeParenIndex - 1), tree->zero);
        if (str.size() - closeParenIndex - 1 > 0) {
            tree->one = new HuffmanNode(nullptr, nullptr);
            cout << "str.size() - closeParenIndex - 1: " << str.size() - closeParenIndex - 1 << endl;
            recreateTreeFromHeaderHelper(str.substr(closeParenIndex + 1, str.size() - closeParenIndex - 1), tree->one);
        }
    } else if (str[0] == '.') {
        if (tree->zero == nullptr) {
            tree->zero = new HuffmanNode(nullptr, nullptr);
            tree->zero->ch = str[1];
            recreateTreeFromHeaderHelper(str.substr(2), tree);
        } else {
            tree->one = new HuffmanNode(nullptr, nullptr);
            tree->one->ch = str[1];
            recreateTreeFromHeaderHelper(str.substr(2), tree);
        }
    }
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
    if (t == nullptr) {
        return;
    }
    if (t->isLeaf()) {
        delete t;
    } else {
        freeTree(t->zero);
        freeTree(t->one);
    }
    t = nullptr;
}

void compress(istream& input, HuffmanOutputFile& output) {
    Map<char, int> freqTable = buildFrequencyTable(input);
    rewindStream(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    string header = flattenTreeToHeader(encodingTree);
    output.writeHeader(header);
    Map<char, string> encodingMap = buildEncodingMap(encodingTree);
    freeTree(encodingTree);
    char ch;
    while (input.get(ch)) {
        string encoding = encodingMap.get(ch);
        for (int i = 0; i < encoding.size(); i++) {
            output.writeBit(charToInteger(encoding[i]));
        }
    }
}

void decompress(HuffmanInputFile& input, ostream& output)
{
    // TODO: implement this function
    (void) input;
    (void) output;
}
