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

// exposedCubeFaces is an output parameter
// vecBoard is an output parameter
void processChars(Vector<string>& cubes, int rows, int cols,
                  Set<char>& exposedCubeFaces,
                  Vector<Vector<char> >& vecBoard) {
    int charPlaced = 0;
    for (int row = 0; row < rows; row++) {
        Vector<char> colVec;
        for (int col = 0; col < cols; col++) {
            int cubeFace = randomInteger(0, cubes[charPlaced].size() - 1);
            // swap elements so that placed char is always at zero element
            // this enables us to easily iteratr over $cubes and know
            // what is on the board and where
            char temp = cubes[charPlaced][cubeFace];
            cubes[charPlaced][cubeFace] = cubes[charPlaced][0];
            cubes[charPlaced][0] = temp;
            
            // store the exposed char into a set for use later in the program
            exposedCubeFaces.add(temp);
            
            // store the exposed char into a 2-dimensional vector representing
            //   the board for use later in the program
            colVec.add(temp);
            
            // place face-up char from cube onto board
            labelCube(row, col, temp);
            charPlaced++;
        }
        vecBoard.add(colVec);
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
        if (input == "") break;
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

/***** Task 3: Find a given word on the board *****/
struct Coordinate {
    int colNum;
    int rowNum;
};

// is word present when starting form a certain location
bool isWordPresentFrom(const string& word, Vector<Coordinate>& outputPath,
                       int rowN, int colN, const int boardSideLen,
                       Vector<Vector<char> > board) {
    const char EXAMINED_CUBE = '0';

    if (rowN < 0 || rowN >= boardSideLen || colN < 0 || colN >= boardSideLen) {
        // Base Case: attempting to explore a path off the board, fail ASAP
        return false;
    } else if (word.size() == 0) {
        // Base Case: path to find word exists
        return true;
    } else if (board[rowN][colN] != word[0]) {
        // Base Case: dead end path as first char doesn't exist at this
        //   location, quit ASAP
        return false;
    } else if (board[rowN][colN] == EXAMINED_CUBE) {
        // Base Case: already explored this cube in the same tree-path of
        //   the recursive backtracking
        return false;
    }

    // mark a char taken in this path board[] to repeat duplicate usage
    board[rowN][colN] = EXAMINED_CUBE;
    for (int col = -1; col <= 1; col++) {
        for (int row = -1; row <= 1; row++) {
            if (isWordPresentFrom(word.substr(1), outputPath,
                                  rowN + row, colN + col,
                                  boardSideLen, board)) {
                // record coordinate of successful path to find word
                Coordinate path;
                path.colNum = colN;
                path.rowNum = rowN;
                outputPath.add(path);
                return true;
            }
        }
    }
    return false;
}

bool isWordPresent(const string& word, Vector<Coordinate>& outputPath,
                   const Vector<Vector<char> > & board,
                   const int boardSideLen) {
    for (int col = 0; col < boardSideLen; col++) {
        for (int row = 0; row < boardSideLen; row++) {
            if (isWordPresentFrom(word, outputPath, row, col,
                                  boardSideLen, board)) {
                return true;
            }
        }
    }
    return false;
}

bool quickWordPresenceCheck(const string& word, const int maxWordLen,
                            const Set<char>& exposedCubeFaces) {
    if (word.size() > (maxWordLen * maxWordLen)) return false;
    
    for (int i = 0; i < word.size(); i++) {
        if (!exposedCubeFaces.contains(word[i])) return false;
    }
    return true;
}

/***** Task 4: Find all words on the board (computer's turn) *****/
void isWordFormedFrom(string soFar, const int colN, const int rowN,
                      Set<string>& foundWords, Vector<Vector<char> > board,
                      const int boardSideLen, const int minWordLen,
                      const Lexicon& lex) {
    const char EXAMINED_CUBE = '0';
    if (rowN < 0 || rowN >= boardSideLen || colN < 0 || colN >= boardSideLen) {
        // Base Case: attempting to explore a path off the board, fail ASAP
        return;
    } else if (!lex.containsPrefix(soFar)) {
        // Base Case: no possibility of finding a valid word in this branch
        return;
    } else if (soFar.size() >= minWordLen && lex.contains(soFar)) {
        foundWords.add(soFar);
    } else if (board[rowN][colN] == EXAMINED_CUBE) {
        // Base Case: already explored this cube in the same tree-path of
        //   the recursive backtracking
        return;
    }

    // add to the explored string path
    soFar += board[rowN][colN];
    // mark a char taken in this path board[] to repeat duplicate usage
    board[rowN][colN] = EXAMINED_CUBE;
    for (int col = -1; col <= 1; col++) {
        for (int row = -1; row <= 1; row++) {
            isWordFormedFrom(soFar, colN + col, rowN + row,
                             foundWords, board, boardSideLen,
                             minWordLen, lex);
        }
    }
}

Set<string> getAllWords(Vector<Vector<char> >& vecBoard,
                        const int boardSideLen,
                        const int minWordLen,
                        Lexicon& lex) {
    Set<string> result;
    string seed = "";
    for (int col = 0; col < boardSideLen; col++) {
        for (int row = 0; row < boardSideLen; row++) {
            Set<string> found;
            isWordFormedFrom(seed, col, row, found,
                             vecBoard, boardSideLen,
                             minWordLen, lex);
            result += found;
        }
    }
    return result;
}

int main() {
    /***** Task 1: Cube Setup, Board Drawing, and Cube Shaking *****/
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    bool needInstructions = askBoolQuestion("", "Do you need instructions? ");
    if (needInstructions) {
        giveInstructions();
    }
    bool bigBoggie = askBoolQuestion("You can choose standard Boggle (4x4 grid)"
                                     " or Big Boggle (5x5).",
                                     "Would you like Big Boogle? ");
    int bWidth;
    if (bigBoggie) {
        bWidth = 5;
    } else {
        bWidth = 4;
    }
    const int SIDE_LEN = bWidth;
    drawBoard(SIDE_LEN, SIDE_LEN);

    bool forceBoardConfig = askBoolQuestion("",
                                            "Do you want to force the board"
                                            " configuration? ");

    Vector<string> cubes;
    if (forceBoardConfig) {
        cubes = getManualCubes(SIDE_LEN);
    } else {
        cubes = shuffleCubes(STANDARD_CUBES,
                             SIDE_LEN * SIDE_LEN);
    }

    Set<char> exposedCubeFaces;
    Vector<Vector<char> > vecBoard;
    processChars(cubes, SIDE_LEN, SIDE_LEN, exposedCubeFaces, vecBoard);
    
    /***** Task 2: Human's Turn (except for finding words on the board) *****/
    Set<string> enteredWords;
    Lexicon dict("EnglishWords.dat");
    const int MIN_WORD_LENGTH = 4;
    
    cout << "OK, take all the time you want and find all the words ";
    cout << "you can! Signal that you're finished by entering an empty line.";
    cout << endl << endl;
    
    string nextWord;
    Vector<Coordinate> outPath;
    while (true) {
        nextWord = getNextHumanWord(enteredWords, dict, MIN_WORD_LENGTH);
        if (nextWord == "") break;
        // record that a given word has been guessed so it cannot be guessed
        //   again, regardless of whether it is valid (save duplicate work)
        enteredWords.add(nextWord);
        
        /***** Task 3: Find a given word on the board *****/
        // before doing an expensive recursive operation, check to see if
        //   all the letters of the word are present on the board; if not,
        //   quit immmediately. also check for impossibly long words
        // short-circut evaluation ensures we only incur expensive recursive
        //   computation if absolutely necessary
        if (!quickWordPresenceCheck(nextWord, SIDE_LEN, exposedCubeFaces) ||
            !isWordPresent(nextWord, outPath, vecBoard, SIDE_LEN)) {
            cout << "You can't make that word!" << endl;
            continue;
        } else {
            for (int i = 0; i < outPath.size(); i++) {
                highlightCube(outPath[i].rowNum, outPath[i].colNum, true);
            }
            recordWordForPlayer(nextWord, HUMAN);
            pause(1000);
            for (int i = 0; i < outPath.size(); i++) {
                highlightCube(outPath[i].rowNum, outPath[i].colNum, false);
            }
        }
        outPath.clear();
    }

    /***** Task 4: Find all words on the board (computer's turn) *****/
    cout << "Computer's Turn" << endl;
    Set<string> allWords = getAllWords(vecBoard, SIDE_LEN,
                                       MIN_WORD_LENGTH, dict);
    Set<string> remainingWords = allWords - enteredWords;
    cout << "Found " << allWords.size() << " words!" << endl;
    cout << allWords << endl;
    foreach(string foundWord in allWords) {
        recordWordForPlayer(foundWord, COMPUTER);
    }
    cout << "End" << endl;
    return 0;
}
