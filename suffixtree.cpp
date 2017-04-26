#include "suffixtree.h"
#include <iostream>

using namespace std;

SuffixTree::SuffixTree() {

    root = nullptr;
}

SuffixTree::SuffixTree(char* all_text, int text_len) {
    root = new Node();

    
    string line = "";
    for (int i = 0; i < text_len; i++ ) {
        char curr = all_text[i];
        if ((unsigned int) curr > 126) {
            
        }
        else {
            if (i % 1000000 == 0 ) cout << i << endl;
            if (curr == '\n') {              
                //line += '\n';
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

    condenseTree(root);
    //printNodes(root,"");
    cout << "Compressed" << endl;
    augmentScore(root); 
    cout << "Augmented" << endl;
    
    //cout << root->kids[(T){"na"}]->hs_node->total << endl;
    //cout << root->kids[(T){"na"}]->total << endl;
    
    //printNodes(root->kids[(T){"na"}]->hs_node,"");
    //cout << root->hs_node->total << endl;
    //cout << root->kids[(T){"banana"}]->total << endl;
    /*test("$banana");
    test("banana");
    test("nanana");
    test("nana");
    test("ana");
    test("na");
    test("a");
    */
    // gotta use flag in find to determine if at end of string 
}

int SuffixTree::calcScore(string label,Node* n ) {
    int val = 0;
    if ( n->total.length() > 80 ) {
        return val;

    }
    int leaves = n->subtree_leaves;
    if (n->flag != 0) leaves++;
    val = n->total.length() * leaves;
    //cout << n->total << " " << val << endl;
    return val;
}

Node* SuffixTree::augmentScore(Node* n) {
    if (n == nullptr) {
        return nullptr;
    }
    Node* highest = nullptr;
    int highest_val = 0;
    for (map<T,Node*>::iterator it = n->kids.begin(); it != n->kids.end(); ++it) {
         Node* q = augmentScore(it->second);
         if (q->flag != 0){
             int temp = calcScore("",q);

         if ( temp > highest_val) { // first param to calc can be used for label size
            
            highest_val = temp;
            highest = q;
            
            // maybe store finished string at node or key to get to it parent works just need the key to get down to it
            
        }
        }
    }
    n->hs_node = highest;
    return n;
}

void SuffixTree::test(string a) {
    Node* result = find(a, root);
    if (result != nullptr) {
        cout << result->letter_depth << " " << result->end_depth << endl;;
    }
    else cout << "NotFound" << endl;
}

SuffixTree::~SuffixTree() {


}

Node* SuffixTree::find(string p, Node* n) {
    //cout << n->letter_depth << endl;
    if (p.length() < 1) {
        return nullptr;
    }
    
    string f = string(1,p.at(0));
    /*
    cout << "options: " << endl;
    for ( map<T,Node*>::iterator it = n->kids.begin(); it != n->kids.end() ; ++it) {
        cout << it->first.label << endl;

    }
    cout << "end options" << endl;
    */

        if (n->kids.count((T){f}) > 0) {
            map<T,Node*>::iterator temp = n->kids.find((T){f});
            
            if (temp->first.label.length() > p.length()) {
                return n->hs_node;
            }


            bool last = false;
            if (temp->first.label.length() == p.length()) {
                last = true;
            }
            string q;
            for (int j = 0; j < temp->first.label.length(); j++) {
                q = string(1,p.at(j));
                if ( q != string(1,temp->first.label.at(j))) {
                    cout << "Not found" << endl;
                    return nullptr;
                }
            }
            if ( last ) {
                cout << "result = " << temp->second->total << endl;
                return temp->second;

            }

            q = p.substr(temp->first.label.length());
            return find(q, temp->second);
                
        }
        else {
        }
    cout << "NOTFIUND" << endl;
    return nullptr;

}

void SuffixTree::insert(string l) {
    insert(root, l, 0, nullptr, ""); 
}

// set is ordered alphabetically so can use binary search
Node* SuffixTree::insert(Node* t, string l, int depth, Node* parent, string prev) {
    if (l.length() == 0) {
        return nullptr;
    }
    else {
        t->subtree_leaves++;
        string f = string(1,l.at(0));
        depth++;
        string rest = l.substr(1);
        if (t->kids.count((T){f}) > 0) {
            if (rest.length() == 0) {
                t->kids[(T){f}]->flag = 1;
            }
            // maybe if statement to determine to increment leaves
            insert(t->kids[(T){f}], rest, depth, t,f);   
        }
        else {
            Node* n = new Node();
            if (l.length() == 1 ) {
                n->flag = 1;
            }
            else {
                n->flag = 0;   
            }
            n->total += t->total + f;
            n->parent = parent;
            n->letter_depth = depth;
            n->end_depth = depth+1;
            t->kids.insert(pair<T, Node*> ((T){f},n));
            //cout << "-First : " << f << " Rest: " << rest << endl;
            if (l.length() > 1 ) {
                insert(n,rest, depth,t,f);
            }
        }



    }

    return t;
}

void SuffixTree::condenseTree(Node* n) {
    if (n == nullptr) {
        return;
    }
    for (map<T,Node*>::iterator it = n->kids.begin(); it != n->kids.end(); ++it) {
        map<T, Node*>::iterator recurseOn = it;

        while (recurseOn->second->kids.size() == 1 && recurseOn->second->flag == 0) {
            map<T, Node*>::iterator child = recurseOn->second->kids.begin();
            string label = child->first.label;
            
            recurseOn = child;
            int start_depth = it->second->letter_depth;
            
            // Might be messing up order with += label
            it->second = child->second; // might be unnessesary recurse on variable
            it->second->parent = n; 
            it->first.label += label;
            it->second->end_depth = it->second->letter_depth;
            it->second->letter_depth = start_depth;
            //cout << it->second->total << endl; 
            //might not need this it->second->subtree_leaves;
            
        }
        
        condenseTree(recurseOn->second);
    }
    
}

void SuffixTree::printNodes(Node* r, string offset) {
    cout << "-----" << endl;
    if (r == nullptr) {
        return;
    }
    else {
        for ( map<T,Node*>::iterator i = r->kids.begin();
          i != r->kids.end(); ++i) {
            if (i->second->flag) cout << i->first.label << " " << i->second->subtree_leaves << endl;
            else { 
                cout << i->first.label << " " << i->second->subtree_leaves;
            }
            
            cout << endl;
        }
    }
}
