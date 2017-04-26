#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include "node.h"

class SuffixTree {
    public:
    
    Node* root;


    SuffixTree(); 
    SuffixTree(char* all_text, int text_len);
    ~SuffixTree();

    Node* find(string p, Node*);
    void insert(string l);
    Node* insert(Node* t, string l, int depth, Node* parent, string prev);
    void condenseTree(Node*);
    void printNodes(Node *, string);

    Node* augmentScore(Node*);
    int calcScore(string,Node*);

    void test(string);

};


#endif
