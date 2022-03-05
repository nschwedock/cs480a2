//Nathan Schwedock
//822910056
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

#include "populatetree.h"
#include "countwords.h"
#include "exec_status.h"

using namespace std;

#define NUM_MANDATORY_ARGUMENTS 2
#define NUM_OPTIONAL_ARGUMENTS 3

#define MARK_CTR_START 1

int main(int argc, char **argv){
    /*takes care of command line arguments*/
    int p_hat = DEFAULT_NUMOF_MARKS;
    int h_hat = DEFAULT_HASHMARKINTERVAL;
    int n_hat = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;

    //if both files are included in the arguments, run the program
    if (argc > NUM_MANDATORY_ARGUMENTS){
        string dictFile = argv[1];
        string testFile = argv[2];
        
        //I understand why 2 might seem like a magic number,
        //but it's purpose is to account for possible spaces between the flag and its number
        //i.e. "-n 3" vs "-n3"
        if (argc > NUM_MANDATORY_ARGUMENTS + 2*NUM_OPTIONAL_ARGUMENTS + 1){
            std::cerr << "too many arguments provided" << std::endl;
            exit(1);
        }

        int Option;
        while ( (Option = getopt(argc, argv, "p:h:n:")) != EOF) {
            switch (Option)
            {
            case 'p' :
                p_hat = atoi(optarg);
                break;
            
            case 'h' :
                h_hat = atoi(optarg);
                break;

            case 'n' :
                n_hat = atoi(optarg);
                break;
            
            default:
                p_hat = -1;
                h_hat = -1;
                n_hat = -1;
                cerr << "flag arguments must be followed by an integer" << endl;
                exit(1);
                break;
            }
        }

        //assuming all optional arguments have been passed correctly, 
        //it's time to check if they meet their respective requirments
        if (p_hat < MIN_NUMOF_MARKS){
            cerr << "Number of progress marks must be a number and at least 10" << endl;
            exit(1);
        }
        if (h_hat < MIN_HASHMARKINTERVAL || h_hat > MAX_HASHMARKINTERVAL){
            cerr << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
            exit(1);
        }
        if (n_hat < DEFAULT_MINNUM_OFWORDS_WITHAPREFIX){
            cerr << "Minimum number of words with a prefix must be a number an at least 1" << endl;
            exit(1);
        }

        //EXEC_STATUS struct stores information to be used by all threads concurrently
        EXEC_STATUS *exec_status = new EXEC_STATUS;

        //putting trie data structure in the exec_status
        exec_status->trie = Trie();

        //storing the names of the files in the exec_status
        exec_status->filePath[DICTSRCFILEINDEX] = dictFile;
        exec_status->filePath[TESTFILEINDEX] = testFile;

        //used for indicating whether or not a certain thread is completed yet
        exec_status->taskCompleted[DICTSRCFILEINDEX] = false;
        exec_status->taskCompleted[TESTFILEINDEX] = false;

        //storing the optional command line arguments in exec_status
        exec_status->numOfProgressMarks = p_hat;
        exec_status->hashmarkInterval = h_hat;
        exec_status->minNumOfWordsWithAPrefixForPrinting = n_hat;

        //storing the amount of words for each file in exec_status, 
        //used for displaying the number of words that were processed by each thread
        exec_status->wordCountInFile[DICTSRCFILEINDEX] = 0;
        exec_status->wordCountInFile[TESTFILEINDEX] = 0;

        //storing the number of chars processed from the whole file,
        //used for properly displaying the progress bar
        exec_status->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] = (long *) malloc(sizeof(long));
        *exec_status->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] = 0;
        exec_status->numOfCharsProcessedFromFile[TESTFILEINDEX] = (long *) malloc(sizeof(long));
        *exec_status->numOfCharsProcessedFromFile[TESTFILEINDEX] = 0;

        //stores the number of bytes in each file
        struct stat buf;
        stat(exec_status->filePath[DICTSRCFILEINDEX].c_str(), &buf);
        exec_status->totalNumOfCharsInFile[DICTSRCFILEINDEX] = (long) buf.st_size;

        stat(exec_status->filePath[TESTFILEINDEX].c_str(), &buf);
        exec_status->totalNumOfCharsInFile[TESTFILEINDEX] = (long) buf.st_size;

        //declaring and creating the two threads for this program
        pthread_t populateThread, countThread;
        pthread_create(&populateThread, NULL, &populatetree, (void *) exec_status);
        pthread_create(&countThread, NULL, &countwords, (void *) exec_status);

        long interval = exec_status->totalNumOfCharsInFile[DICTSRCFILEINDEX] / exec_status->numOfProgressMarks;
        long markCtr = MARK_CTR_START;
        while (!exec_status->taskCompleted[DICTSRCFILEINDEX]){
            //waiting for populate tree thread to finish
            if ((*exec_status->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] > (interval * markCtr)) && (markCtr <= exec_status->numOfProgressMarks)){
                if (markCtr % exec_status->hashmarkInterval == 0){
                    cout.flush() << "#";
                }
                else{
                    cout.flush() << "-";
                }
                markCtr++;
            }
        }

        interval = exec_status->totalNumOfCharsInFile[TESTFILEINDEX] / exec_status->numOfProgressMarks;
        markCtr = MARK_CTR_START;
        while (!exec_status->taskCompleted[TESTFILEINDEX]){
            //waiting for count words thread to finish
            if ((*exec_status->numOfCharsProcessedFromFile[TESTFILEINDEX] > (interval * markCtr)) && (markCtr <= exec_status->numOfProgressMarks)){
                if (markCtr % exec_status->hashmarkInterval == 0){
                    cout.flush() << "#";
                }
                else{
                    cout.flush() << "-";
                }
                markCtr++;
            }
        }
    }
    else{
        //incorrect number of arguments given
        cout << "Incorrect number of arguments given" << endl;
    }
    
    return 0;
}