//Nathan Schwedock
//822910056
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "countwords.h"
#include "exec_status.h"

#define BYTE 1

using namespace std;

const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

void * countwords(void *VoidPtr){
    EXEC_STATUS *exec = (EXEC_STATUS *) VoidPtr;

    while (!exec->taskCompleted[DICTSRCFILEINDEX]){
        //waiting for the populate tree thread to finish
    }

    ifstream inFile;

    //open the test file.  If it doesn't open properly, give an errorCode
    inFile.open(exec->filePath[TESTFILEINDEX]);
    if (!inFile){
        cerr << "unable to open <<" << exec->filePath[TESTFILEINDEX] << ">>" << endl;
        exit(1);
    }

    inFile.close();

    string line;
    char *words;

    ifstream input_file(exec->filePath[TESTFILEINDEX]);

    ofstream output_file ("countwords_output.txt");

    //read in line by line and increments number of processed chars
    while (getline(input_file, line) && output_file.is_open()){
        char *line_c = new char[line.length() + 1];
        strcpy(line_c, line.c_str());
        *exec->numOfCharsProcessedFromFile[TESTFILEINDEX]+= (line.length() + BYTE);
        //tokenize each line into each word
        words = strtok(line_c, delimiters);
        while (words != nullptr){
            exec->wordCountInFile[TESTFILEINDEX]++;
            //counts each word and spits it out
            int num_count = exec->trie.countWords(exec->trie.findEndingNode(std::string(words)));
            if (num_count >= exec->minNumOfWordsWithAPrefixForPrinting){
                output_file << words << " " << num_count << endl;
            }
            words = strtok(NULL, delimiters);
        }
    }

    output_file.close();
    input_file.close();

    std::cout << "\nThere are " << exec->wordCountInFile[TESTFILEINDEX] << " words in " << exec->filePath[TESTFILEINDEX] << "." << std::endl;

    exec->taskCompleted[TESTFILEINDEX] = true;
    return nullptr;
}