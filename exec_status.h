//Nathan Schwedock
//822910056
#ifndef exec_status_H
#define exec_status_H

#include "dicttree.h"

/* default number of progress marks for representing 100% progress */ 
#define DEFAULT_NUMOF_MARKS 50 
 
/* minimum number of progress marks for representing 100% progress */ 
#define MIN_NUMOF_MARKS 10 
 
/* place hash marks in the progress bar every N marks */ 
#define DEFAULT_HASHMARKINTERVAL  10 

/* minimum number of hash marks for each interval in the progress bar */
#define MIN_HASHMARKINTERVAL 1

/* maximum number of hash marks for each interval in the progress bar */
#define MAX_HASHMARKINTERVAL 10
 
/* default minimum number of dictionary words starting from a prefix for printing or 
writing to the output */ 
#define DEFAULT_MINNUM_OFWORDS_WITHAPREFIX  1

#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1

typedef struct {
    /** 
    * root node of the dictionary tree 
    */ 
    Trie trie;

    std::string filePath[NUMOFFILES];

    /** 
    * parameters for printing progress bar  
    */ 
    int numOfProgressMarks; 
 
    int hashmarkInterval;

    /** 
    * print a word and its count to the output file only if the  
    * number of dictionary words starting from the word is equal to or  
    * greater than this number  
    **/ 
    int minNumOfWordsWithAPrefixForPrinting;

    long *numOfCharsProcessedFromFile[NUMOFFILES];

    long totalNumOfCharsInFile[NUMOFFILES];

    long wordCountInFile[NUMOFFILES];

    bool taskCompleted[NUMOFFILES];
}EXEC_STATUS;

#endif