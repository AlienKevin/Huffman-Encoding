#include "encoding.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "stack.h"
#include "filelib.h"

void buildEncodingMapHelper(HuffmanNode* encodingTree, string encoding, Map<char, string>& encodingMap);
void recreateTreeFromHeaderHelper(string str, HuffmanNode*& tree);

/**
 * @brief Build frequency table for a given input
 * @param input the input to encode
 * @return the frequency table as a map
 */
Map<char, int> buildFrequencyTable(istream& input) {
    Map<char, int> table = Map<char, int>();
    char ch;
    while (input.get(ch)) {
        table[ch] ++;
    }
    return table;
}

/**
 * @brief Build an encoding tree for a given frequency table
 * @param freqTable the input frequency table
 * @return the root of a Huffman encoding tree
 */
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
        root = new HuffmanNode(node2, node1);
        int rootPriority = node1Priority + node2Priority;
        // put the root back to the forest
        forest.enqueue(root, rootPriority);
    }
    return root;
}

/**
 * @brief Flatten the encoding tree into a string header for storage
 * @param t the root of a Huffman encoding tree
 * @return the string header of the input encoding tree
 */
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

/**
 * @brief Recreate a Huffman encoding tree from a string header
 * @param str the string header to recreate from
 * @return the root of a Huffman encoding tree
 */
HuffmanNode* recreateTreeFromHeader(string str) {
    HuffmanNode* tree = nullptr;
    recreateTreeFromHeaderHelper(str, tree);
    return tree;
}

/**
 * @brief Find the matching right parenthesis of the starting left parenthesis
 * @param str the string to search in
 * @return the index of the matching right parenthesis, -1 if not found
 */
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

/**
 * @brief Helper function for recreateTreeFromHeader
 * @param str the string header to recreate from
 * @param tree the Huffman encoding tree to recreate
 */
void recreateTreeFromHeaderHelper(string str, HuffmanNode*& tree) {
    // Reach the end of header string, tree is complete
    if (str.size() == 0) {
        return;
    }
    // Reach an intermediate node
    if (str[0] == '(') {
        if (tree == nullptr) {
            tree = new HuffmanNode(nullptr, nullptr);
        }
        int closingParenIndex = findMatchingParenthesis(str);
        // The inside string is split into two parts, i.e. # of intermediate nodes >= 1
        if (str.size() - closingParenIndex - 1 > 0) {
            // create a new intermediate node for the first part of the string
            tree->zero = new HuffmanNode(nullptr, nullptr);
            // create a new intermediate node if the second part of the string is parenthesized
            if (str[str.size() - 1] == ')') {
                tree->one = new HuffmanNode(nullptr, nullptr);
            }
            // create subtree for the first part of the string
            recreateTreeFromHeaderHelper(str.substr(1, closingParenIndex - 1), tree->zero);
            // create subtree for the second part of the string
            recreateTreeFromHeaderHelper(str.substr(closingParenIndex + 1, str.size() - closingParenIndex - 1), tree->one);
        } else { // The inside string is either: see (1) and (2)
            // (1): one intermediate node
            if (str[1] == '(') {
                recreateTreeFromHeaderHelper(str.substr(1, str.size() - 2), tree);
            } else { // (2): one leaf + (one leaf or one intermediate node)
                // Add character in the leaf to tree
                recreateTreeFromHeaderHelper(str.substr(1, 2), tree->zero);
                // Add the rest string (one leaf or one intermediate node) to tree
                recreateTreeFromHeaderHelper(str.substr(3, str.size() - 4), tree->one);
            }
        }
    // reach a leaf node containing one character
    } else if (str[0] == '.') {
        if (tree == nullptr) { // end of tree
            tree = new HuffmanNode(str[1]);
        } else if (tree->zero == nullptr) { // first fill in the 'zero' (left) branch
            tree->zero = new HuffmanNode(str[1]);
            recreateTreeFromHeaderHelper(str.substr(2), tree->one);
        }else { // then fill in the 'one' (right) branch
            tree->one = new HuffmanNode(str[1]);
        }
    }
}

/**
 * @brief Build an encoding map for a Huffman encoding tree
 * @param encodingTree the encoding tree to create map for
 * @return the encoding map
 */
Map<char, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<char, string> map = Map<char, string>();
    buildEncodingMapHelper(encodingTree, "", map);
    return map;
}

/**
 * @brief Helper function for buildEncodingMap
 * @param encodingTree the encoding tree to create map for
 * @param encoding the string encoding for a character (at a leaf node)
 * @return the encoding map
 */
void buildEncodingMapHelper(HuffmanNode* encodingTree, string encoding, Map<char, string>& encodingMap) {
    if (encodingTree->isLeaf()) {
        encodingMap[encodingTree->ch] = encoding;
    } else {
        buildEncodingMapHelper(encodingTree->zero, encoding + "0", encodingMap);
        buildEncodingMapHelper(encodingTree->one, encoding + "1", encodingMap);
    }
}

/**
 * @brief Free the allocated memory used in encoding/decoding process
 * @param t the root of the encoding tree
 */
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

/**
 * @brief Compress an input to a Huffman-encoded output
 * @param input the input to compress
 * @param output the compressed output in Huffman encoding
 */
void compress(istream& input, HuffmanOutputFile& output) {
    Map<char, int> freqTable = buildFrequencyTable(input);
    rewindStream(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    // first write the header
    string header = flattenTreeToHeader(encodingTree);
    output.writeHeader(header);
    // then write the encoded characters
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

/**
 * @brief Decompress a Huffman-encoded input to original format
 * @param input the input to decompress
 * @param output the decompressed output in original format
 */
void decompress(HuffmanInputFile& input, ostream& output) {
    string header = input.readHeader();
    HuffmanNode* encodingTree = recreateTreeFromHeader(header);
    int bit = input.readBit();
    HuffmanNode* currentNode = encodingTree;
    while (bit != -1) {
        if (bit == 0) {
            currentNode = currentNode->zero;
        } else if (bit == 1) {
            currentNode = currentNode->one;
        }
        // found a leaf with character in the tree
        if (currentNode->isLeaf()) {
            output << currentNode->ch;
            // reset currentNode to the root of the tree
            // to continue looking for new characters
            currentNode = encodingTree;
        }
        bit = input.readBit();
    }
    freeTree(encodingTree);
}
