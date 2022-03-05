//Nathan Schwedock
//822910056
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "populatetree.h"
#include "exec_status.h"

#define BYTE 1

using namespace std;

void * populatetree(void *VoidPtr){
    EXEC_STATUS *exec = (EXEC_STATUS *) VoidPtr;

    ifstream inFile;
    
    string word;

    inFile.open(exec->filePath[DICTSRCFILEINDEX]);
    if (!inFile){
        cerr << "unable to open <<" << exec->filePath[DICTSRCFILEINDEX] << ">>" << endl;
        exit(1);
    }

    while (!inFile.eof()){
        inFile >> word;
        *exec->numOfCharsProcessedFromFile[DICTSRCFILEINDEX]+= (word.length() + BYTE);
        //add each new word to the dictionary tree
        exec->trie.addWord(word);
        exec->wordCountInFile[DICTSRCFILEINDEX]++;
    }
    exec->wordCountInFile[DICTSRCFILEINDEX]--;

    inFile.close();

    std::cout << "\nThere are " << exec->wordCountInFile[DICTSRCFILEINDEX] << " words in " << exec->filePath[DICTSRCFILEINDEX] << "." << std::endl;

    exec->taskCompleted[DICTSRCFILEINDEX] = true;
    return nullptr;
}