#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdint>
#include <iostream>
#include "chip8.h"

constexpr int SCALE = 4;



int main()
{
    sf::RenderWindow window(sf::VideoMode(64*SCALE, 32*SCALE), "CHIP-8", sf::Style::Default);
    sf::Time clk_period = sf::milliseconds(200);
    sf::Clock clock;
    sf::Time elapsed;
    uint64_t cycles{};

    chip8 chip8{ };
    chip8.reset();

    sf::Image pixelArray;
    sf::Texture screen;
    sf::Sprite  sprite;

    pixelArray.create(64, 32, sf::Color::Black);
    sprite.setScale(sf::Vector2f((float)SCALE, (float)SCALE));
    sprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    std::cout << "memdump preload:" << std::endl;

    for (auto &byte : chip8.mem) {
        int width{ };
        std::cout << std::hex <<byte;
        width++;

        if (width > 32) {
            width = 0;
            std::cout << std::endl;
        }
    }

    chip8.loadProgram("splash.ch8");

    std::cout << "memdump:" << std::endl;

    for (auto &byte : chip8.mem) {
        int width{ };
        std::cout << std::hex <<byte;
        width++;

        if (width > 32) {
            width = 0;
            std::cout << std::endl;
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update keypad state if window in focus

        // cycle main clock at 540hz
        elapsed = clock.getElapsedTime();
        if (elapsed >= clk_period) {
            clock.restart();
            chip8.runCycle();
            std::cout << "it ran a cycle?" << std::endl;
            ++cycles;
        }

        // run sound/delay timers at 60hz
        if (!(cycles % 9)) {
            chip8.runTimers();
        }
        
        // draw when video memory updated
        if (chip8.drawFlag) {
            std::cout << "draw flag" << std::endl;
            uint32_t x{0};
            uint32_t y{0};

            for (auto &pixel : chip8.display) {
                std::cout << "entered the px array for loop " <<std::endl;
                pixelArray.setPixel(x, y, (pixel == 0 ? sf::Color::Black : sf::Color::White));
                std::cout << "set pixel " << x << ", " << y << std::endl;
                ++x;
                if (x == 64) {
                    x = 0;
                    ++y;
                }
                if (y == 32) break; // should never execute
            }

            std::cout << "didn't explode from pixel array" << std::endl;

            screen.loadFromImage(pixelArray);
            std::cout << "didn't explode from loading texture" << std::endl;
            sprite.setTexture(screen);
            std::cout << "didn't explode from setting sprite texture" << std::endl;


            window.clear(sf::Color::Black);
            window.draw(sprite);
            window.display();
            std::cout << "didn't explode from drawing sprite" << std::endl;

            chip8.drawFlag &= 0x00;
        }

        // play sound
        if (chip8.audioIsPlaying()) {
            ;
        }
    }
}