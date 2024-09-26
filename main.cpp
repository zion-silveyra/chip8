#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <cstdint>
#include <iostream>
#include "chip8.h"

constexpr int SCALE = 16;

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(64*SCALE, 32*SCALE), "CHIP-8", sf::Style::Default);
    window.setKeyRepeatEnabled(false);
    uint8_t windowInFocus{1};
    sf::Time clk_period = sf::microseconds(1850);
    sf::Clock clock;
    sf::Time elapsed;
    uint64_t cycles{};

    chip8 chip8{ };
    chip8.reset();

    sf::Image pixelArray;
    sf::Texture screen;
    sf::Sprite  sprite;

    sf::Sound audio;
    sf::SoundBuffer audioBuf;


    pixelArray.create(64, 32, sf::Color::Black);
    sprite.setScale(sf::Vector2f((float)SCALE, (float)SCALE));
    sprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    chip8.loadProgram(argv[1]);

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::LostFocus) {
                windowInFocus &= 0x00;
            }
            if (event.type == sf::Event::GainedFocus) {
                windowInFocus |= 0x01;
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::BackSpace:
                    chip8.reset();
                    chip8.loadProgram(argv[1]);
                    break;
                case sf::Keyboard::Num1: // c8 1
                    chip8.keypad |= 0x01 << 0x1;
                    break;
                case sf::Keyboard::Num2: // c8 2
                    chip8.keypad |= 0x01 << 0x2;
                    break;
                case sf::Keyboard::Num3: // c8 3
                    chip8.keypad |= 0x01 << 0x3;
                    break;
                case sf::Keyboard::Num4: // c8 c
                    chip8.keypad |= 0x01 << 0xc;
                    break;
                case sf::Keyboard::Q:    // c8 4
                    chip8.keypad |= 0x01 << 0x4;
                    break;
                case sf::Keyboard::W:    // c8 5
                    chip8.keypad |= 0x01 << 0x5;
                    break;
                case sf::Keyboard::E:    // c8 6
                    chip8.keypad |= 0x01 << 0x6;
                    break;
                case sf::Keyboard::R:    // c8 d
                    chip8.keypad |= 0x01 << 0xd;
                    break;
                case sf::Keyboard::A:    // c8 7
                    chip8.keypad |= 0x01 << 0x7;
                    break;
                case sf::Keyboard::S:    // c8 8
                    chip8.keypad |= 0x01 << 0x8;
                    break;
                case sf::Keyboard::D:    // c8 9
                    chip8.keypad |= 0x01 << 0x9;
                    break;
                case sf::Keyboard::F:    // c8 e
                    chip8.keypad |= 0x01 << 0xe;
                    break;
                case sf::Keyboard::Z:    // c8 a
                    chip8.keypad |= 0x01 << 0xa;
                    break;
                case sf::Keyboard::X:    // c8 0
                    chip8.keypad |= 0x01 << 0x0;
                    break;
                case sf::Keyboard::C:    // c8 b
                    chip8.keypad |= 0x01 << 0xb;
                    break;
                case sf::Keyboard::V:    // c8 f
                    chip8.keypad |= 0x01 << 0xf;
                    break;
                }

                //std::cout << "new keypad state: " << chip8.keypad << std::endl;
            }
            if (event.type == sf::Event::KeyReleased) {
                //std::cout << "key released" << std::endl;
                switch (event.key.code) {
                case sf::Keyboard::Num1: // c8 1
                    chip8.keypad ^= 0x01 << 0x1;
                    break;
                case sf::Keyboard::Num2: // c8 2
                    chip8.keypad ^= 0x01 << 0x2;
                    break;
                case sf::Keyboard::Num3: // c8 3
                    chip8.keypad ^= 0x01 << 0x3;
                    break;
                case sf::Keyboard::Num4: // c8 c
                    chip8.keypad ^= 0x01 << 0xc;
                    break;
                case sf::Keyboard::Q:    // c8 4
                    chip8.keypad ^= 0x01 << 0x4;
                    break;
                case sf::Keyboard::W:    // c8 5
                    chip8.keypad ^= 0x01 << 0x5;
                    break;
                case sf::Keyboard::E:    // c8 6
                    chip8.keypad ^= 0x01 << 0x6;
                    break;
                case sf::Keyboard::R:    // c8 d
                    chip8.keypad ^= 0x01 << 0xd;
                    break;
                case sf::Keyboard::A:    // c8 7
                    chip8.keypad ^= 0x01 << 0x7;
                    break;
                case sf::Keyboard::S:    // c8 8
                    chip8.keypad ^= 0x01 << 0x8;
                    break;
                case sf::Keyboard::D:    // c8 9
                    chip8.keypad ^= 0x01 << 0x9;
                    break;
                case sf::Keyboard::F:    // c8 e
                    chip8.keypad ^= 0x01 << 0xe;
                    break;
                case sf::Keyboard::Z:    // c8 a
                    chip8.keypad ^= 0x01 << 0xa;
                    break;
                case sf::Keyboard::X:    // c8 0
                    chip8.keypad ^= 0x01 << 0x0;
                    break;
                case sf::Keyboard::C:    // c8 b
                    chip8.keypad ^= 0x01 << 0xb;
                    break;
                case sf::Keyboard::V:    // c8 f
                    chip8.keypad ^= 0x01 << 0xf;
                    break;
                }
            }
            //std::cout << "new keypad state: " << chip8.keypad << std::endl;
        }

       
        // cycle main clock at 540hz
        elapsed = clock.getElapsedTime();
        if ((elapsed >= clk_period)) {
            clock.restart();
            chip8.runCycle();
            ++cycles;
        }

        // run sound/delay timers at 60hz
        if (!(cycles % 9)) {
            chip8.runTimers();
        }
        
        // draw when video memory updated
        if (chip8.drawFlag) {
            sf::Color drawColor;
            uint16_t vramIndex;
            uint32_t x{0};
            uint32_t y{0};

            for (y=0;y<32;++y) {
                for (x=0;x<64;++x) {
                    vramIndex = (64 * y) + x;

                    drawColor = (chip8.display[vramIndex] == 0) ? sf::Color::Black : sf::Color::White;
                    pixelArray.setPixel(x, y, drawColor);
                }
            }

            screen.loadFromImage(pixelArray);
            sprite.setTexture(screen);

            window.clear(sf::Color::Black);
            window.draw(sprite);
            window.display();

            chip8.drawFlag &= 0x00;
        }

        // play sound
        if (chip8.audioIsPlaying()) {
            ;
        }
    }
}