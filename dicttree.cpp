//Nathan Schwedock
//822910056
#include <iostream>
#include "dicttree.h"

#define ASCII_APOS 39       //ascii value for apostrophe
#define ASCII_LOWER 97      //ascii value for 'a'

//Adding Node children
//Also returns if the whole word has been added
bool Trie::addWord(std::string word){
    Node *curr = &root;
    for (int i = 0; i < word.size(); i++){
        char c = word[i];
        //apostrophe is the next char
        if ((int)(c) == ASCII_APOS){
            if (curr->children[NUM_CHARS-1] == NULL){
                curr->children[NUM_CHARS-1] = new Node();
            }
            curr = curr->children[NUM_CHARS-1];
        }
        //Upper case letter is the next char
        else if ((int)(c) < ASCII_LOWER){
            if (curr->children[c - 'A'] == NULL){
                curr->children[c - 'A'] = new Node();
            }
            curr = curr->children[c - 'A']; 
        }
        //lower case letter is the next char
        else{
            if (curr->children[c - 'a'] == NULL){
                curr->children[c - 'a'] = new Node();
            }
            curr = curr->children[c - 'a'];
        }
    }
    curr->isWord = true;

    return true;
}

//returns the node pointer that ends the string if found
Node* Trie::findEndingNode(std::string word){
            Node *curr = &root;
            for (int i = 0; i < word.size(); i++) {
                char c = word[i];
                //if apostrophe is the next char, go into the element for that
                if ((int)(c) == ASCII_APOS){
                    if (curr->children[NUM_CHARS-1] == NULL){
                        return new Node();
                    }
                    curr = curr->children[NUM_CHARS-1];
                }
                //if an upper case letter is the next char, go into the element for that
                else if ((int)(c) < ASCII_LOWER){
                    if (curr->children[int(c - 'A')] == NULL){
                        return new Node();
                    }
                    curr = curr->children[int(c - 'A')];
                }
                //if a lower case letter is the next char, go into the element for that
                else{
                    if (curr->children[int(c - 'a')] == NULL) {
                        return new Node();
                    }
                    curr = curr->children[int(c - 'a')];
                }
            }
            //return the node pointer of the last character in the string
            return curr;
}

//counts the number of words in the trie that exist from a starting word/node
int Trie::countWords(Node* node){//starts at t node address
    int sum = 0;
    if (node->isWord){
        sum++;
    }
    for (int i = 0; i < NUM_CHARS; i++) {
        if (node->children[i] != nullptr) {
            Node *temp = node->children[i];
            sum += countWords(temp);
        }
    }
    return sum;
}



