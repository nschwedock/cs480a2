//Nathan Schwedock
//822910056
#ifndef dicctree_H
#define dicttree_H

#define NUM_CHARS 27

#include <string>


class Node{
    public:
        bool isWord;
        Node *children[NUM_CHARS];

        //Node constructor
        Node(){
            isWord = false;
            for (int i = 0; i < NUM_CHARS; i++){
                children[i] = NULL;
            }
        }
};

class Trie{
    private:
        Node root;
    public:
        //Trie constructor which holds a root node
        Trie(){
            root = *(new Node());
        }

        //Adding Node children
        //Also returns if the whole word has been added
        bool addWord(std::string word);

        //returns the node pointer that ends the string if found
        Node* findEndingNode(std::string word);

        //counts the number of words in the trie that exist from a starting word/node
        int countWords(Node* node);

};

#endif