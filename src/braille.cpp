#include <braille.hpp>
#include <map>
#include <iostream>
#include <vector>

void draw_letter(int letterNum, std::vector<std::vector<int>> &letter_pos, int cusCaret[2]) {
    int brailleLetterMap[26][6] = {
        { 1, 0,
          0, 0,  // a
          0, 0},
        { 1, 0,
          1, 0,  // b
          0, 0},
        { 1, 1,
          0, 0,  // c
          0, 0},
        { 1, 1,
          0, 1,  // d
          0, 0},
        { 1, 0,
          0, 1,  // e
          0, 0},
        { 1, 1,
          1, 0,  // f
          0, 0},
        { 1, 1,
          1, 1,  // g
          0, 0},
        { 1, 0,
          1, 1,  // h
          0, 0},
        { 0, 1,
          1, 0,  // i
          0, 0},
        { 0, 1,
          1, 1,  // j
          0, 0},
        { 1, 0,
          0, 0,  // k
          1, 0},
        { 1, 0,
          1, 0,  // l
          1, 0},
        { 1, 1,
          0, 0,  // m
          1, 0},
        { 1, 1,
          0, 1,  // n
          1, 0},
        { 1, 0,
          0, 1,  // o
          1, 0},
        { 1, 1,
          1, 0,  // p
          1, 0},
        { 1, 1,
          1, 1,  // q
          1, 0},
        { 1, 0,
          1, 1,  // r
          1, 0},
        { 0, 1,
          1, 0,  // s
          1, 0},
        { 0, 1,
          1, 1,  // t
          1, 0},
        { 1, 0,
          0, 0,  // u
          1, 1},
        { 1, 0,
          1, 0,  // v
          1, 1},
        { 0, 1,
          1, 1,  // w
          0, 1},
        { 1, 1,
          0, 0,  // x
          1, 1},
        { 1, 1,
          0, 1,  // y
          1, 1},
        { 1, 0,
          0, 1,  // z
          1, 1}
    };
    int brailleNumberMap[10][6] = {
      { 0, 1,
        1, 1,  // 0
        0, 0},
      { 1, 0,
        0, 0,  // 1
        0, 0},
      { 1, 0,
        1, 0,  // 2
        0, 0},
      { 1, 1,
        0, 0,  // 3
        0, 0},
      { 1, 1,
        0, 1,  // 4
        0, 0},
      { 1, 0,
        0, 1,  // 5
        0, 0},
      { 1, 1,
        1, 0,  // 6
        0, 0},
      { 1, 1,
        1, 1,  // 7
        0, 0},
      { 1, 0,
        1, 1,  // 8
        0, 0},
      { 0, 1,
        1, 0,  // 9
        0, 0},
    };
    /*Semicolon , Comma , Period ,
  Apostrophe , Slash , Backslash , Grave ,
  Equal , Hyphen , Space ,*/
    int brailleCharMap[10][6] = { // all need to start with bolded braille
      { 0, 0,
        1, 0,  // semicolon
        1, 0},
      { 0, 0,
        1, 0,  // comma
        0, 0},
      { 0, 0,
        1, 1,  // period
        0, 1},
      { 0, 0,
        0, 0,  // apostrophe
        1, 0},
      { 0, 1,
        0, 0,  // slash
        1, 0},
      { 1, 0,
        0, 0,  // backslash
        0, 1},
      { 1, 1,
        1, 1,  // grave
        1, 1},
      { 1, 1,
        1, 1,  // equal
        1, 1},
      { 0, 0,
        0, 0,  // hyphen
        1, 1},
      { 0, 0,
        0, 0,  // space
        0, 0},
    };

    int caret[2] = { 0, 0 };


    if (cusCaret == nullptr) {
        cusCaret = caret;
    }
    int x = cusCaret[0];
    int y = cusCaret[1];
    int* letterBraille;
    if (letterNum < 25) {
      letterBraille = brailleLetterMap[letterNum];
    } else if (letterNum >= 26 && letterNum < 36) {
      letterBraille = brailleNumberMap[letterNum-26];
    } else if (letterNum >= 48 && letterNum < 60) {
      letterBraille = brailleCharMap[letterNum-48];
    }
    
    std::vector<std::vector<int>> letter = {
        {x, y, letterBraille[0]}, {x+1, y, letterBraille[1]},
        {x, y+1, letterBraille[2]}, {x+1, y+1, letterBraille[3]},
        {x, y+2, letterBraille[4]}, {x+1, y+2, letterBraille[5]},
    };
    letter_pos = letter;
    std::cout << letter_pos[0][0] << std::endl;
}