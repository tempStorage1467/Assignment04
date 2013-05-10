/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */
 
#include <iostream>
#include <string>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "set.h"
using namespace std;

/*
At the heart of the program are two recursive functions that
find words on the board, one for the human player and another for the computer
 
 these two recursive functions need to be different... don't overlap them
 */

////////// CONSTANTS //////////

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

////////// FUNCTION IMPLEMENTATION //////////
/* Task 1: Cube Setup, Board Drawing, and Cube Shaking */
Vector<string> arrayToVector(const string str[], int length) {
    Vector<string> vec(length);
    for (int i = 0; i < length; i++) {
        vec[i] = str[i];
    }
    return vec;
}

Vector<string> shuffleCubes(const string iCubes[], int numCubes) {
    Vector<string> cubes = arrayToVector(iCubes, numCubes);

    for (int i = 0; i < cubes.size(); i++) {
        int posToSwap = randomInteger(i, cubes.size() - 1);
        string temp = cubes[posToSwap];
        cubes[posToSwap] = cubes[i];
        cubes[i] = temp;
    }
    return cubes;
}

void placeChars(Vector<string>& cubes, int rows, int cols) {
    int charPlaced = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int cubeFace = randomInteger(0, cubes[charPlaced].size() - 1);
            // swap elements so that placed char is always at zero element
            // this enables us to easily iteratr over $cubes and know
            // what is on the board and where
            char temp = cubes[charPlaced][cubeFace];
            cubes[charPlaced][cubeFace] = cubes[charPlaced][0];
            cubes[charPlaced][0] = temp;
            labelCube(row, col, temp);
            charPlaced++;
        }
    }
}

void strToUpperCase(string& str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
}

bool isValidCustomBoardConfiguration(string& input, int boardSize) {
    if (input.size() != boardSize * boardSize) return false;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] < 65 || input[i] > 90) {
            return false;
        }
    }
    return true;
}

Vector<string> getManualCubes(const int boardSideLen) {
    cout << "" << endl;
    cout << "Enter a " << (boardSideLen * boardSideLen) << "-character string ";
    cout << "to identify which letters you want on cubes. ";
    cout << "The first " << boardSideLen << " letters are the cubes on the top";
    cout << " row from left to right, the next " << boardSideLen << "letters";
    cout << " are on the second row, and so on." << endl;
    
    string input;
    while (input != "y" && input != "yes" &&
           input != "n" && input != "no") {
        input = getLine("Enter the string: ");
        strToUpperCase(input);
        if (isValidCustomBoardConfiguration(input, boardSideLen)) {
            break;
        } else {
            cout << "String must include " << boardSideLen * boardSideLen;
            cout << " characters!" << endl;
        }
    }

    // copy each char of input string as a string element of vector
    Vector<string> cubes(input.size());
    for (int i = 0; i < input.size(); i++) {
        // convert char to string safely and put into array
        cubes[i] += input[i];
    }
    return cubes;
}

bool askBoolQuestion(string preface, string question) {
    cout << endl;
    if (preface.length() > 0) {
        cout << preface << endl;
    }
    string input;
    while (input != "Y" && input != "YES" &&
           input != "Y" && input != "NO") {
        input = getLine(question);
        strToUpperCase(input);
    }
    return (input == "Y" || input == "YES");
}

/* Main program */

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}


/* Task 2: Human's Turn (except for finding words on the board) */
string getNextHumanWord(Set<string>& enteredWords,
                        Lexicon& dict,
                        const int minWordLen) {
    string input;
    bool promptAgain = true;
    while (promptAgain) {
        input = getLine("Enter the string: ");
        input = toUpperCase(input);
        if (enteredWords.contains(input)) {
            // check if user already entered this word, either as a guess (in
            //   which case we shouldn't waste time checking it again or as
            //   a successful word)
            cout << "You're already guessed that!" << endl;
            continue;
        }
        if (input.size() < minWordLen) {
            cout << "I'm sorry, but we have standards." << endl;
            cout << "That word doesn't meet the minimum word length." << endl;
            continue;
        }
        if (!dict.contains(input)) {
            cout << "That's not a word!" << endl;
            continue;
        }
        promptAgain = false;
    }
    return input;
}

/* Task 3: Find a given word on the board */
void getBoardArr(char* board[][], int boardSideLen) {

}

struct Coordinate {
    int colNum;
    int rowNum;
    char letter;
};

bool isWordPresentFrom(const string& word, Vector<Coordinate>& outputPath,
                       int xPos, int yPos, const int boardSideLen,
                       Vector<Vector<char> > board) {
    const char EXAMINED_CUBE = '0';

    if (xPos < 0 || xPos >= boardSideLen || yPos < 0 || yPos >= boardSideLen) {
        // Base Case: attempting to explore a path off the board, fail ASAP
        return false;
    } else if (word.size() == 0) {
        // Base Case: path to find word exists
        return true;
    } else if (board[xPos][yPos] != word[0]) {
        // Base Case: dead end path as first char doesn't exist at this
        //   location, quit ASAP
        return false;
    } else if (board[xPos][yPos] == EXAMINED_CUBE) {
        // Base Case: already explored this cube in the same tree-path of
        //   the recursive backtracking
        return false;
    }
    
    // first char of the word to find exists at this (x, y) coordinate, so
    //   record it
    Coordinate path;
    path.colNum = yPos;
    path.rowNum = xPos;
    path.letter = board[xPos][yPos];
    outputPath.add(path);
    board[xPos][yPos] = EXAMINED_CUBE;
    for (int col = -1; col <= 1; col++) {
        for (int row = -1; row <= 1; row++) {
            // mark a char taken in this path board[] to repeat duplicate usage
            if (isWordPresentFrom(word.substr(1), outputPath, xPos + col, yPos + row,
                                  boardSideLen, board)) {
                    return true;
                }
        }
    }
    return false;
}

bool isWordPresent(const string& word, Vector<Coordinate>& outputPath,
                   const Vector<string>& cubes, const int boardSideLen) {
    // create a two dimensional vector of chars with letters on board
    //   as it will be much easier to navigate this than a vector
    int i = 0;
    Vector<Vector<char> > board;
    for (int col = 0; col < boardSideLen; col++) {
        Vector<char> rowVec;
        for (int row = 0; row < boardSideLen; row++) {
            rowVec.add(cubes[i][0]);
            i++;
        }
        board.add(rowVec);
    }

    for (int col = 0; col < boardSideLen; col++) {
        for (int row = 0; row < boardSideLen; row++) {
            if (isWordPresentFrom(word, outputPath, col, row,
                                  boardSideLen, board)) {
                return true;
            }
        }
    }
    return false;
}

bool quickWordPresenceCheck(const string& word, const Vector<string>& cubes,
                            const int maxWordLen) {
    if (word.size() > (maxWordLen * maxWordLen)) return false;

    Set<char> exposedCubes;
    for (int i = 0; i < cubes.size(); i++) {
        exposedCubes.add(cubes[i][0]);
    }
    
    for (int i = 0; i < word.size(); i++) {
        if (!exposedCubes.contains(word[i])) return false;
    }
    return true;
}

int main() {
    /* Task 1: Cube Setup, Board Drawing, and Cube Shaking */
    
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    bool needInstructions = false;//askBoolQuestion("", "Do you need instructions? ");
    if (needInstructions) {
        giveInstructions();
    }
    bool bigBoggie = false;//askBoolQuestion("You can choose standard Boggle (4x4 grid)"
                           //          " or Big Boggle (5x5).",
                           //          "Would you like Big Boogle? ");
    int bWidth;
    if (bigBoggie) {
        bWidth = 5;
    } else {
        bWidth = 4;
    }
    const int BOARD_SIDE_LEN = bWidth;
    drawBoard(BOARD_SIDE_LEN, BOARD_SIDE_LEN);

    bool forceBoardConfig = true;//askBoolQuestion("",
                                 //               "Do you want to force the board"
                                 //               " configuration? ");

    Vector<string> cubes;
    if (forceBoardConfig) {
        cubes = getManualCubes(BOARD_SIDE_LEN);
    } else {
        cubes = shuffleCubes(STANDARD_CUBES,
                             BOARD_SIDE_LEN * BOARD_SIDE_LEN);
    }

    placeChars(cubes, BOARD_SIDE_LEN, BOARD_SIDE_LEN);
    
    /* Task 2: Human's Turn (except for finding words on the board) */
    Set<string> enteredWords;
    Lexicon dict("EnglishWords.dat");
    const int MIN_WORD_LENGTH = 4;
    
    cout << "OK, take all the time you want and find all the words ";
    cout << "you can! Signal that you're finished by entering an empty line.";
    cout << endl << endl;
    
    string nextWord = " ";
    Vector<Coordinate> outputPath;
    while (nextWord != "") {
        nextWord = getNextHumanWord(enteredWords, dict, MIN_WORD_LENGTH);
        // record that a given word has been guessed so it cannot be guessed
        //   again, regardless of whether it is valid (save duplicate work)
        enteredWords.add(nextWord);
        
        /* Task 3: Find a given word on the board */
        // before doing an expensive recursive operation, check to see if
        //   all the letters of the word are present on the board; if not,
        //   quit immmediately. also check for impossibly long words
        // short-circut evaluation ensures we only incur expensive recursive
        //   computation if absolutely necessary
        if (!quickWordPresenceCheck(nextWord, cubes, BOARD_SIDE_LEN) ||
            !isWordPresent(nextWord, outputPath, cubes, BOARD_SIDE_LEN)) {
            cout << "You can't make that word!" << endl;
            continue;
        } else {
            cout << "Word Found!" << endl;
            cout << "Word Path: " << outputPath.size() << endl;
            for (int i = 0; i < outputPath.size(); i++) {
                cout << outputPath[i].colNum << ", " << outputPath[i].rowNum << ", " << outputPath[i].letter << endl;
                highlightCube(outputPath[i].rowNum, outputPath[i].colNum, true);
            }
        }
        
    }

    return 0;
}
