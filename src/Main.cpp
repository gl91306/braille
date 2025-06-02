#include <iostream>

#include <SFML/Graphics.hpp>
#include <braille.hpp>
#include <vector>

using namespace sf;

int main()
{
    std::cout << "Hello World!" << std::endl;
    float SCALE = 5.0;
    float WIDTH = ((32.0*2.0)+1.0)*SCALE;
    float HEIGHT = ((64.0*2.0)+1.0)*SCALE;
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Braille Phone Software Development Kit (v0.0)");
    std::cout << WIDTH*SCALE << " " <<  HEIGHT*SCALE << std::endl;

    // Phone Plate
    RectangleShape rectangle(Vector2f(WIDTH, HEIGHT));
    Color backgroundColor = Color(200, 200, 200, 255);
    rectangle.setFillColor(backgroundColor);
    float centRectPosX = (WIDTH/2)-(rectangle.getSize().x/2);
    float centRectPosY = (HEIGHT/2)-(rectangle.getSize().y/2);
    rectangle.setPosition(Vector2f(centRectPosX, centRectPosY));

    // Phone pixels
    RectangleShape* pixels[32][64];
    for (int x = 0; x < 32; x++) {
        RectangleShape row[64];
        for (int y = 0; y < 64; y++) {
            float yale = 1; // 16
            int pixelSize = 1;
            RectangleShape* pixel = new RectangleShape(Vector2f(pixelSize*SCALE, pixelSize*SCALE));
            pixel->setFillColor(backgroundColor);
            pixel->setPosition(x*SCALE + SCALE*x + SCALE, y*SCALE + SCALE*y + SCALE);
            pixels[x][y] = pixel;
        }
    }

    // vector of int arrays with size 4
    std::vector<std::vector<int>> rectangles;
    // x, y, width, height, caretX, caretY
    // draw the main screen
    rectangles.push_back({0, 0, 32, 54, 2, 2}); // 0 will always be the first half
    rectangles.push_back({0, 54, 32, 10, 2, 56}); // 1 will always be the second half

    // everything after will be specific to screen

    int selectedRectangle = 0;

    // set_pixel function
    auto set_pixel = [backgroundColor, pixels](int x, int y, bool on) 
    { 
        RectangleShape* pixel = pixels[x][y];
        if (on) {
            pixel->setFillColor(Color::Black);
        } else {
            pixel->setFillColor(backgroundColor);
        }
        
    };
    auto draw_image = [SCALE, &set_pixel](const std::string& filename, int offsetX, int offsetY, float scaleFactor) -> std::vector<int> {
        sf::Image image;
        if (!image.loadFromFile(filename)) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            return {0,0};
        }
    
        sf::Vector2u imgSize = image.getSize();
        int targetWidth = static_cast<int>(imgSize.x * scaleFactor);
        int targetHeight = static_cast<int>(imgSize.y * scaleFactor);
    
        for (int x = 0; x < targetWidth; ++x) {
            for (int y = 0; y < targetHeight; ++y) {
                int srcX = static_cast<int>(x / scaleFactor);
                int srcY = static_cast<int>(y / scaleFactor);
    
                if (srcX >= imgSize.x || srcY >= imgSize.y) continue;
    
                sf::Color color = image.getPixel(srcX, srcY);
                int avg = (color.r + color.g + color.b) / 3;
                bool on = (avg < 128); // Closer to black than white
                int px = offsetX + x;
                int py = offsetY + y;
    
                if (px >= 0 && px < 32 && py >= 0 && py < 64) {
                    set_pixel(px, py, on);
                }
            }
        }
        return {targetWidth, targetHeight};
    };

    auto draw_rectangle = [set_pixel](int x, int y, int width, int height, int filled) 
    {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (filled == 1 || i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                    set_pixel(x + i, y + j, true);
                } else {
                    //set_pixel(x + i, y + j, false);
                }
            }
        }
    };

    auto charToKey = [](char c) -> sf::Keyboard::Key {
        if (c >= 'a' && c <= 'z') {
            return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + (c - 'a'));
        }
        if (c >= 'A' && c <= 'Z') {
            return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + (c - 'A'));
        }
        if (c >= '0' && c <= '9') {
            return static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + (c - '0'));
        }
        return sf::Keyboard::Unknown;
    };
    

    auto write_string_cleared = [&](const std::string& text, int rectIndex) {
        int letterLen = 2;
        int letterHeight = 3;
    
        // Reset caret to initial position
        int caretX = rectangles[rectIndex][0] + letterLen;
        int caretY = rectangles[rectIndex][1] + letterLen;
    
        // Clear old text by filling the area with spaces
        int rectWidth = rectangles[rectIndex][2] - rectangles[rectIndex][0];
        int rectHeight = rectangles[rectIndex][3] - rectangles[rectIndex][1];
        for (int y = 0; y < rectHeight; y += letterHeight + 1) {
            for (int x = 0; x < rectWidth; x += letterLen + 1) {
                std::vector<std::vector<int>> letter_pos(6, std::vector<int>(3, 0));
                draw_letter(charToKey(' '), letter_pos, (int[2]){caretX + x, caretY + y});
                for (int i = 0; i < 6; i++) {
                    set_pixel(letter_pos[i][0], letter_pos[i][1], false);
                }
            }
        }
    
        // Reset caret
        caretX = rectangles[rectIndex][0] + letterLen;
        caretY = rectangles[rectIndex][1] + letterLen;
    
        for (char c : text) {
            std::vector<std::vector<int>> letter_pos(6, std::vector<int>(3, 0));
            draw_letter(charToKey(c), letter_pos, (int[2]){caretX, caretY});
            for (int i = 0; i < 6; i++) {
                set_pixel(letter_pos[i][0], letter_pos[i][1], letter_pos[i][2]);
            }
    
            caretX += letterLen + 1;
            if (caretX > rectangles[rectIndex][2] - letterLen + 1) {
                caretX = rectangles[rectIndex][0] + letterLen;
                caretY += letterHeight + 1;
                if (caretY > rectangles[rectIndex][3] - letterHeight + 1) {
                    caretX = rectangles[rectIndex][0] + letterLen;
                    caretY = rectangles[rectIndex][1] + letterLen;
                }
            }
        }
    
        rectangles[rectIndex][4] = caretX;
        rectangles[rectIndex][5] = caretY;
    };

    auto write_string_append = [&](const std::string& text, int rectIndex) {
        int letterLen = 2;
        int letterHeight = 3;
    
        int caretX = rectangles[rectIndex][4];
        int caretY = rectangles[rectIndex][5];
    
        for (char c : text) {
            std::vector<std::vector<int>> letter_pos(6, std::vector<int>(3, 0));
            draw_letter(charToKey(c), letter_pos, (int[2]){caretX, caretY});
            for (int i = 0; i < 6; i++) {
                set_pixel(letter_pos[i][0], letter_pos[i][1], letter_pos[i][2]);
            }
    
            caretX += letterLen + 1;
            if (caretX > rectangles[rectIndex][2] - letterLen + 1) {
                caretX = rectangles[rectIndex][0] + letterLen;
                caretY += letterHeight + 1;
                if (caretY > rectangles[rectIndex][3] - letterHeight + 1) {
                    caretX = rectangles[rectIndex][0] + letterLen;
                    caretY = rectangles[rectIndex][1] + letterLen;
                }
            }
        }
    
        rectangles[rectIndex][4] = caretX;
        rectangles[rectIndex][5] = caretY;
    };
    
    auto set_menu = [&rectangles, write_string_append, write_string_cleared](int menuNum) {

        while (rectangles.size() > 2) {
            rectangles.pop_back();
        }
        if (menuNum == 0) { // phone
            std::cout << "BRUH" << std::endl;
            int p = 6;
            rectangles.push_back({2, 2+p, 8, 7, 4, 4+p});
            rectangles.push_back({12, 2+p, 8, 7, 14, 4+p});
            rectangles.push_back({22, 2+p, 8, 7, 24, 4+p});

            rectangles.push_back({2, 11+p, 8, 7, 4, 13+p});
            rectangles.push_back({12, 11+p, 8, 7, 14, 13+p});
            rectangles.push_back({22, 11+p, 8, 7, 24, 13+p});

            rectangles.push_back({2, 20+p, 8, 7, 4, 22+p});
            rectangles.push_back({12, 20+p, 8, 7, 14, 22+p});
            rectangles.push_back({22, 20+p, 8, 7, 24, 22+p});

            rectangles.push_back({12, 29+p, 8, 7, 14, 31+p});

            rectangles.push_back({2, 38+p, 8, 7, 4, 40+p});
            rectangles.push_back({22, 38+p, 8, 7, 24, 40+p});

            // set text phone
            // clear screen
            write_string_cleared(" ", 0);
            // write numbers in each box
            for (int i = 1; i < 10; i++) {
                write_string_cleared(std::to_string(i), 1+i);
            }
            write_string_cleared("0", 11);
            // 
        }
    };

    // testing

    /*RectangleShape* pixel = new RectangleShape(Vector2f(10, 10));
    pixel->setFillColor(Color::Green);
    pixel->setPosition(0,0);
    pixels[0][0] = pixel;

    set_pixel(0,0,false);
    set_pixel(0,1,false);
    set_pixel(0,62,false);*/

    while (window.isOpen())
    {
        // Rectagle Draw
        for (int i = 0; i < rectangles.size(); i++) {
            draw_rectangle(rectangles[i][0], rectangles[i][1], rectangles[i][2], rectangles[i][3], 0);
        }

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == 60) {
                    // semicolon bings cursor to menu
                    if (selectedRectangle == rectangles.size() - 1) {
                        selectedRectangle = 0;
                    } else {
                        selectedRectangle++;
                    }
                    continue;
                }
                std::cout << event.key.code << std::endl;
                std::vector<std::vector<int>> letter_pos {{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}}};
                int caret[2] = {rectangles[selectedRectangle][4], rectangles[selectedRectangle][5]};
                int letterLen = 2;
                int letterHeight = 3;
                if (event.key.code == Keyboard::P) {
                    set_menu(0);
                    //auto letterDimensions = draw_image("phone.png", caret[0], caret[1], 1); // draw phone icon
                    //letterLen = letterDimensions[0];
                    //letterHeight = letterDimensions[1];
                } else {
                    // draw letters
                    draw_letter(event.key.code, letter_pos, caret);
                    std::cout << "caret: " << caret[0] << " " << caret[1] << std::endl;
                    for (int i=0; i < 6; i++) {
                        set_pixel(letter_pos[i][0], letter_pos[i][1], letter_pos[i][2]);
                    }
                }
                caret[0] = caret[0] + letterLen + 1;
                if (caret[0] > rectangles[selectedRectangle][2] - letterLen + 1) {
                    std::cout << "asdjfoisjfd" << std::endl;
                    caret[0] = rectangles[selectedRectangle][0] + letterLen;
                    caret[1] = caret[1] + letterHeight + 1;
                    if (caret[1] > rectangles[selectedRectangle][3] - letterHeight + 1) {
                        caret[0] = rectangles[selectedRectangle][0] + letterLen;
                        caret[1] = rectangles[selectedRectangle][1] + letterLen;
                    }
                }
                rectangles[selectedRectangle][4] = caret[0];
                rectangles[selectedRectangle][5] = caret[1];
            }
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rectangle);
        // Draw all pixels
        for (int x = 0; x < 32; x++) {
            for (int y = 0; y < 64; y++) {
                window.draw(*pixels[x][y]); // Draw the object pointed to by the pointer
            }
        }
        
        window.display();;
    }

    return 0;
}