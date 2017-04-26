#ifndef NODE_H
#define NODE_H
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;
struct T {
        //mutable string a;
        mutable string label;
        bool operator<(const T& rhs) const { return label.at(0) < rhs.label.at(0) ;}
};


class Node {

 
    public :

    
    map<T, Node*> kids;
    Node * parent;

    // augmented vals
    int subtree_leaves;
    int letter_depth;
    int end_depth;
    int flag; // -1 unset 0 off 1 on
    string total;
    Node* hs_node;
    Node() {
        parent = nullptr;
        subtree_leaves = 0;
        letter_depth = 0;
        end_depth = 0;
        flag = -1;
        hs_node = nullptr;
        total = "";
    }


};



#endif

