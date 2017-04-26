#include "suffixtree.h"
#include <iostream>

using namespace std;

SuffixTree::SuffixTree() {

    root = nullptr;
    num_elements = 0;
}

SuffixTree::SuffixTree(char* all_text, int text_len) {
    root = nullptr;
    num_elements = 0;

    
    string line = "";
    for (int i = 0; i < text_len; i++ ) {
        char curr = all_text[i];
        if ((unsigned int) curr > 126) {
            // ignore weird characters should i ignore the whole line?
            
        }
        else {
            if (i % 1000000 == 0 ) cout << i << endl;
            if (curr == '\n') {              
                line += '\0';
                insert(line);
                line = "";
            }
            else {
                if (isalpha(curr)) {
                    curr = tolower(curr);
                }
                line += curr;
            }
        }
        
    }

    for (set<Node*>::iterator it = root->children.begin(); it != root->children.end(); ++it) {
        cout << ((*it)->label) << endl;
    }

    condenseTree(root);
    printNodes(root);
}

SuffixTree::~SuffixTree() {


}

void SuffixTree::insert(string l) {
    root = insert(root, l); 
}

// set is ordered alphabetically so can use binary search
Node* SuffixTree::insert(Node* t, string l) {
    if (l.size() == 0) return nullptr;
    if (t == nullptr) {

        Node* nn = new Node(string(1,l.at(0)));
        if (l.length() > 1) {
            nn->children.insert(insert(nullptr, l.substr(1)));
        }
        return nn;
    }

    // Find if letter exists for it
    set<Node*>::iterator it = t->children.begin();
    for (; it != t->children.end(); ++it) {
        if (l.length() < 1) {
            it = t->children.end();
            break;
        }
        if ((*it)->label == string(1,l.at(0))) {
            break;   
        }
    }


    if ( it != t->children.end()) {
        insert(*it, l.substr(1));
    }
    else {

        t->children.insert(insert(nullptr, l));

    }
    return t;

}

void SuffixTree::condenseTree(Node* n) {
    
    //cout << "label : " << n->label << " "  << n->children.size() << endl;
    
    if (n == nullptr) {
        return;
    }

    for (set<Node*>::iterator it = n->children.begin(); it != n->children.end(); ++it) {
        //cout << (*it)->label << endl;
        if ((*it)->children.size() == 1) {
            // need to move any augmentation too
            Node* q = *(((*it)->children.begin()));
            string temp = (*it)->label + q->label;
            q->label = temp;
            // replace node
            //cout << (*it)->label << " " << temp << " " << endl;
            
            n->children.erase(it);
            n->children.insert(q);

            //cout << temp << endl;
        }
        
        condenseTree((*it));
        

    }



}

void SuffixTree::printNodes(Node* r) {
    if (r == nullptr) {
        return;
    }
    else {
        
        for (set<Node*>::iterator it = r->children.begin(); it != r->children.end(); ++it) {
            cout << ((*it)->label) << endl;
            printNodes(*it);
        }
    
    }
}
