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
    rectangles.push_back({0, 0, 32, 54, 2, 2});
    rectangles.push_back({0, 54, 32, 10, 2, 56});

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

    auto draw_rectangle = [set_pixel](int x, int y, int width, int height, int filled) 
    {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                if (filled == 1 || i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                    set_pixel(x + i, y + j, true);
                } else {
                    set_pixel(x + i, y + j, false);
                }
            }
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

    // Initial Rectagle Draw
    for (int i = 0; i < rectangles.size(); i++) {
        draw_rectangle(rectangles[i][0], rectangles[i][1], rectangles[i][2], rectangles[i][3], 0);
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                std::cout << event.key.code << std::endl;
                std::vector<std::vector<int>> letter_pos {{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}},{{0},{0},{0}}};
                int caret[2] = {rectangles[selectedRectangle][4], rectangles[selectedRectangle][5]};
                draw_letter(event.key.code, letter_pos, caret);
                std::cout << "caret: " << caret[0] << " " << caret[1] << std::endl;
                for (int i=0; i < 6; i++) {
                    set_pixel(letter_pos[i][0], letter_pos[i][1], letter_pos[i][2]);
                }
                caret[0] = caret[0] + 3;
                if (caret[0] > rectangles[selectedRectangle][2] - 3) {
                    std::cout << "asdjfoisjfd" << std::endl;
                    caret[0] = rectangles[selectedRectangle][0] + 2;
                    caret[1] = caret[1] + 4;
                    if (caret[1] > rectangles[selectedRectangle][3] - 4) {
                        caret[0] = rectangles[selectedRectangle][0] + 2;
                        caret[1] = rectangles[selectedRectangle][1] + 2;
                    }
                }
                rectangles[selectedRectangle][4] = caret[0];
                rectangles[selectedRectangle][5] = caret[1];
                if (event.key.code == 48) {
                    selectedRectangle = 1;
                } else if (event.key.code == 51) {
                    selectedRectangle = 0;
                }
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