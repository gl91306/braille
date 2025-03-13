#ifndef BRAILLE_HPP    // To make sure you don't declare the function more than once by including the header multiple times.
#define BRAILLE_HPP

#include <vector>

void draw_letter(int letterNum, std::vector<std::vector<int>> &letter_pos, int cusCaret[2] = nullptr);


#endif