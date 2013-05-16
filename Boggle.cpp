/*
 * File: Boggle.cpp
 * ----------------
 * Name: Eric Beach
 * Section: Dawson Zhou
 * This file is the main starter file for Assignment #4, Boggle.
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
/***** Task 1: Cube Setup, Board Drawing, and Cube Shaking *****/
/*
 * Convert an array of strings into a vector of strings.
 */
Vector<string> arrayToVector(const string str[], int length) {
    Vector<string> vec(length);
    for (int i = 0; i < length; i++) {
        vec[i] = str[i];
    }
    return vec;
}

/*
 * Shuffle cubes such that the order of the cubes is random. This helps
 *   to ensure a random board.
 */
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

/*
 * Process the characters in the shuffled cubes, putting them onto the board
 *   and populating output parameters.
 * $exposedCubeFaces and $vecBoard are both output parameters that will be
 *   populated and used later in the game.
 */
void processChars(Vector<string>& cubes, int rows, int cols,
                  Set<char>& exposedCubeFaces,
                  Vector<Vector<char> >& vecBoard) {
    int cubeCnt = 0;
    for (int row = 0; row < rows; row++) {
        // create a vector representing a column of cubes
        Vector<char> colVec;
        for (int col = 0; col < cols; col++) {
            int selectedCubeFace = randomInteger(0, cubes[cubeCnt].size() - 1);
            // swap elements so that the placed char is always at zero element.
            //   i.e., randomize the char facing up on the cube
            // placing the randomly selected char in the 0th element
            //  enables easy iteration over $cubes later on
            char temp = cubes[cubeCnt][selectedCubeFace];
            cubes[cubeCnt][selectedCubeFace] = cubes[cubeCnt][0];
            cubes[cubeCnt][0] = temp;
            
            // store the exposed char into a set for use later in the program
            exposedCubeFaces.add(temp);
            
            // store the exposed char into a 2-dimensional vector representing
            //   the board for use later in the program
            colVec.add(temp);
            
            // place face-up char from cube onto board
            labelCube(row, col, temp);
            cubeCnt++;
        }
        vecBoard.add(colVec);
    }
}

/*
 * Convert a string to uppercase.
 */
void strToUpperCase(string& str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
}

/*
 * Determine whether a specific string input consisting of characters entered
 *   by the end-user to represent a board configuration is valid. Check
 *   to ensure that the only used characters are uppercase letters.
 */
bool isValidCustomBoardConfiguration(const string& input, int boardLength) {
    if (input.size() != boardLength * boardLength) return false;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] < 65 || input[i] > 90) {
            return false;
        }
    }
    return true;
}

/*
 * Provide instructions to the user on how to enter a manual board
 *   configuration and then receive the input from the user. Run a function
 *   to check the input and if invalid, prompt the user again.
 */
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

    // copy each char of user's custom configured board, which is captured
    //   as an input string, into a string element of vector
    Vector<string> cubes(input.size());
    for (int i = 0; i < input.size(); i++) {
        // convert char, which represents a cube, to string
        //   which is put into vector containing all the cubes
        cubes[i] += input[i];
    }
    return cubes;
}

/*
 * Ask the end user a boolean (i.e., yes/no) question.
 */
bool askBoolQuestion(string preface, string question) {
    cout << endl;
    // if a preface to the question is supplied, print it
    if (preface.length() > 0) {
        cout << preface << endl;
    }
    string input;
    while (input != "Y" && input != "YES" &&
           input != "N" && input != "NO") {
        input = getLine(question);
        strToUpperCase(input);
    }
    return (input == "Y" || input == "YES");
}

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

/***** Task 2: Human's Turn (except for finding words on the board) *****/
/*
 * Ask the end user to provide another guess of a word on the board.
 * Perform basic checking of this guessed word, ensuring that it has not
 *   been guessed before, it is of valid length, and it is a valid word.
 */
string getNextHumanWord(const Set<string>& enteredWords,
                        const Lexicon& dict,
                        const int minWordLen) {
    string input;
    bool promptAgain = true;
    while (promptAgain) {
        input = getLine("Enter the string: ");
        input = toUpperCase(input);
        if (input == "") break;
        if (enteredWords.contains(input)) {
            // check if user already entered this word as a guess (in
            //   which case we shouldn't waste time checking it again or as
            //   a successful word)
            cout << "You're already guessed that!" << endl;
            continue;
        }
        if (input.size() < minWordLen) {
            // check if the word is too short
            cout << "I'm sorry, but we have standards." << endl;
            cout << "That word doesn't meet the minimum word length." << endl;
            continue;
        }
        if (!dict.contains(input)) {
            // check if the word is valid in the dictionary
            cout << "That's not a word!" << endl;
            continue;
        }
        promptAgain = false;
    }
    return input;
}

/***** Task 3: Find a given word on the board *****/
/*
 * A structure to represent a coordinate position on the board.
 */
struct Coordinate {
    int colNum;
    int rowNum;
};

/*
 * Determine whether a sequence of characters (i.e., $word) can be formed
 *   by starting at a given location (i.e., colN, rowN) and trying all
 *   adjacent locations that have not previously been explored in this
 *   recursive branch (which is measured by examining $board for the
 *   presence of an EXAMINED_CUBE).
 *
 * The key recursive insight is that if a word can be formed from a given
 *   location, then the next character in the string (i.e., word[0])
 *   needs to be present at a given location and we then explore all the
 *   adjacent locations for word[1], which we can pass as word.substr(1).
 *   In other words, if "dime" can be formed from 0,0 then a d needs to be at
 *   0,0 and we need to be able to form the word "ime" from an adjacent cube.
 */
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

/*
 * This is the helper function that sets up the calls to the function
 *   that does the heart of Task 3 (i.e., isWordPresentFrom).
 * This function determine whether a word is present when starting from
 *   every possible location on the board.
 */
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

/*
 * Determine whether it is possible for a given guessed word to be present
 *   on the board by checking if it is too long and by checking to ensure
 *   all the required characters are on the board.
 * This quick check improves overall efficiency by preventing more expensive
 *   recursive exploration of the board in search of a word that cannot
 *   possibly be present. For example, if the word is "computer" but the
 *   board does not contain an "r", it would be an extremely expensive
 *   resursive operation to discover this but with a few iterations
 *   over a set, this function would detect it and prevent the
 *   expensive recursive operation.
 */
bool quickWordPresenceCheck(const string& word, const int maxWordLen,
                            const Set<char>& exposedCubeFaces) {
    if (word.size() > (maxWordLen * maxWordLen)) return false;
    
    for (int i = 0; i < word.size(); i++) {
        if (!exposedCubeFaces.contains(word[i])) return false;
    }
    return true;
}

/***** Task 4: Find all words on the board (computer's turn) *****/
/* Find all words that can be formed from a given starting position with a
 *   given part of a word found thus far (i.e., find words from a given location
 *   with a given prefix, stored in $soFar).
 * This function is the heart of Task #4.
 * The key recursive insight is that all the words that can be formed from
 *   (colN, rowN) with a stem of $soFar are all the words that can be formed
 *   from the adjoining locations where $soFar has the current cube's char
 *   added.
 */
void findWordsFrom(string soFar, const int colN, const int rowN,
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
            findWordsFrom(soFar, colN + col, rowN + row,
                             foundWords, board, boardSideLen,
                             minWordLen, lex);
        }
    }
}

/*
 * This is the helper function that sets up the recursive function that
 *   performs the heart of the work in Task 4.
 * This function gets all words that can be formed starting from every
 *   location on the board. It appends all words formed from each location
 *   into a set.
 */
Set<string> getAllWords(Vector<Vector<char> >& vecBoard,
                        const int boardSideLen,
                        const int minWordLen,
                        Lexicon& lex) {
    Set<string> result;
    string seed = "";
    for (int col = 0; col < boardSideLen; col++) {
        for (int row = 0; row < boardSideLen; row++) {
            Set<string> found;
            findWordsFrom(seed, col, row, found,
                             vecBoard, boardSideLen,
                             minWordLen, lex);
            result += found;
        }
    }
    return result;
}

/***** Task 5: Loop to play many games, add polish *****/
/*
 * Play the Boggle game. This allows the end-user to look
 */
void playBoggle(const int SIDE_LEN) {
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
    
    // a set containing all the chars that are facing up on the board.
    //   this is used to perform quick checking of user guesses.
    Set<char> exposedCubeFaces;
    
    // a 2-dimensional vector representing the chars that are shown on the board
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
        if (quickWordPresenceCheck(nextWord, SIDE_LEN, exposedCubeFaces) &&
            isWordPresent(nextWord, outPath, vecBoard, SIDE_LEN)) {
            // guessed word could be found; highlight it on the board and then
            //   remove the highlighting
            for (int i = 0; i < outPath.size(); i++) {
                highlightCube(outPath[i].rowNum, outPath[i].colNum, true);
            }
            recordWordForPlayer(nextWord, HUMAN);
            pause(750);
            for (int i = 0; i < outPath.size(); i++) {
                highlightCube(outPath[i].rowNum, outPath[i].colNum, false);
            }
        } else {
            cout << "You can't make that word!" << endl;
        }
        outPath.clear();
    }
    
    /***** Task 4: Find all words on the board (computer's turn) *****/
    // find all possible words on the board
    Set<string> allWords = getAllWords(vecBoard, SIDE_LEN,
                                       MIN_WORD_LENGTH, dict);
    // remove already guessed words
    Set<string> remainingWords = allWords - enteredWords;
    // print each word onto the screen that the computer found
    foreach(string foundWord in remainingWords) {
        recordWordForPlayer(foundWord, COMPUTER);
    }
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
    
    bool playAgain = true;
    while (playAgain) {
        // in theory, as per the documentation for drawBoard() I should
        //   not need to make this call to clear, but drawBoard() did not
        //   produce the results of clearing, so I manually called it here.
        gw.clear();
        playBoggle(SIDE_LEN);
        playAgain = askBoolQuestion("", "Would you like to play again? ");
    }
    return 0;
}
