#include "chip8.h"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdint>
#include <cmath>

constexpr int SCALE = 16;

void generateSamples(sf::Int16 * destination, int size);
void updateKeypad(sf::Event keyboardEvent, uint16_t &keypad);

int main(int argc, char* argv[])
{
    // ---------- Window ---------- //
    sf::RenderWindow window(sf::VideoMode(64*SCALE, 32*SCALE), "CHIP-8", sf::Style::Default);
    window.setKeyRepeatEnabled(false);
    uint8_t windowInFocus{1};

    // ---------- Clock ---------- //
    sf::Time clk_period = sf::microseconds(1850);
    sf::Clock clock;
    sf::Clock timer;
    sf::Time   clk_elapsed;
    sf::Time timer_elapsed;
    uint64_t cycles{};

    // ---------- Rendering ---------- //
    sf::Image pixelArray;
    sf::Texture screen;
    sf::Sprite  sprite;

    pixelArray.create(64, 32, sf::Color::Black);
    sprite.setScale(sf::Vector2f((float)SCALE, (float)SCALE));
    sprite.setPosition(sf::Vector2f(0.0f, 0.0f));

    // ---------- Audio ---------- //
    constexpr int SAMPLE_RATE = 44100;

    sf::Sound audio;
    audio.setLoop(true);
    sf::SoundBuffer audioBuf;
    
    sf::Int16 audioSamples[SAMPLE_RATE]{}; 

    generateSamples(audioSamples, SAMPLE_RATE);
    audioBuf.loadFromSamples(audioSamples, SAMPLE_RATE, 1, SAMPLE_RATE);
    audio.setBuffer(audioBuf);

    // ---------- Chip 8 ---------- //
    chip8 chip8{};
    chip8.reset();
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
                case sf::Keyboard::Backspace:
                    chip8.reset();
                    chip8.loadProgram(argv[1]);
                    break;
                case sf::Keyboard::Dash:
                    // decrease execution speed
                    clk_period += sf::microseconds(100);
                    std::cout << "New clock period: " << clk_period.asMicroseconds() << std::endl;
                    break;
                case sf::Keyboard::Equal:
                    // increase execution speed
                    clk_period -= sf::microseconds(100);
                    std::cout << "New clock period: " << clk_period.asMicroseconds() << std::endl;
                    break;
                default:
                    updateKeypad(event, chip8.keypad);
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                updateKeypad(event, chip8.keypad);
            }
        }

        // cycle clock, pause when window not in focus
        clk_elapsed = clock.getElapsedTime();
        if ((clk_elapsed >= clk_period && windowInFocus)) {
            clock.restart();
            chip8.runCycle();
            ++cycles;
        }

        // run sound/delay timers at 60hz
        timer_elapsed = timer.getElapsedTime();
        if (timer_elapsed >= sf::microseconds(16667)) {
            timer.restart();
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
        if (chip8.audioIsPlaying() && audio.getStatus() == sf::SoundSource::Stopped) {
            audio.play();
        }
        else if (!chip8.audioIsPlaying() && audio.getStatus() == sf::SoundSource::Playing) {
            audio.stop();    
        }
    }
}

void generateSamples(sf::Int16 * destination, int size)
{
    for (int i{}; i < size; ++i) {
        destination[i] = ((i & 0x80) ? 500000 : 0);
    }
}

void updateKeypad(sf::Event event, uint16_t &keypad)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::Num1: // c8 1
            keypad |= 0x01 << 0x1;
            break;
        case sf::Keyboard::Num2: // c8 2
            keypad |= 0x01 << 0x2;
            break;
        case sf::Keyboard::Num3: // c8 3
            keypad |= 0x01 << 0x3;
            break;
        case sf::Keyboard::Num4: // c8 c
            keypad |= 0x01 << 0xc;
            break;
        case sf::Keyboard::Q:    // c8 4
            keypad |= 0x01 << 0x4;
            break;
        case sf::Keyboard::W:    // c8 5
            keypad |= 0x01 << 0x5;
            break;
        case sf::Keyboard::E:    // c8 6
            keypad |= 0x01 << 0x6;
            break;
        case sf::Keyboard::R:    // c8 d
            keypad |= 0x01 << 0xd;
            break;
        case sf::Keyboard::A:    // c8 7
            keypad |= 0x01 << 0x7;
            break;
        case sf::Keyboard::S:    // c8 8
            keypad |= 0x01 << 0x8;
            break;
        case sf::Keyboard::D:    // c8 9
            keypad |= 0x01 << 0x9;
            break;
        case sf::Keyboard::F:    // c8 e
            keypad |= 0x01 << 0xe;
            break;
        case sf::Keyboard::Z:    // c8 a
            keypad |= 0x01 << 0xa;
            break;
        case sf::Keyboard::X:    // c8 0
            keypad |= 0x01 << 0x0;
            break;
        case sf::Keyboard::C:    // c8 b
            keypad |= 0x01 << 0xb;
            break;
        case sf::Keyboard::V:    // c8 f
            keypad |= 0x01 << 0xf;
            break;
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        //std::cout << "key released" << std::endl;
        switch (event.key.code) {
        case sf::Keyboard::Num1: // c8 1
            keypad ^= 0x01 << 0x1;
            break;
        case sf::Keyboard::Num2: // c8 2
            keypad ^= 0x01 << 0x2;
            break;
        case sf::Keyboard::Num3: // c8 3
            keypad ^= 0x01 << 0x3;
            break;
        case sf::Keyboard::Num4: // c8 c
            keypad ^= 0x01 << 0xc;
            break;
        case sf::Keyboard::Q:    // c8 4
            keypad ^= 0x01 << 0x4;
            break;
        case sf::Keyboard::W:    // c8 5
            keypad ^= 0x01 << 0x5;
            break;
        case sf::Keyboard::E:    // c8 6
            keypad ^= 0x01 << 0x6;
            break;
        case sf::Keyboard::R:    // c8 d
            keypad ^= 0x01 << 0xd;
            break;
        case sf::Keyboard::A:    // c8 7
            keypad ^= 0x01 << 0x7;
            break;
        case sf::Keyboard::S:    // c8 8
            keypad ^= 0x01 << 0x8;
            break;
        case sf::Keyboard::D:    // c8 9
            keypad ^= 0x01 << 0x9;
            break;
        case sf::Keyboard::F:    // c8 e
            keypad ^= 0x01 << 0xe;
            break;
        case sf::Keyboard::Z:    // c8 a
            keypad ^= 0x01 << 0xa;
            break;
        case sf::Keyboard::X:    // c8 0
            keypad ^= 0x01 << 0x0;
            break;
        case sf::Keyboard::C:    // c8 b
            keypad ^= 0x01 << 0xb;
            break;
        case sf::Keyboard::V:    // c8 f
            keypad ^= 0x01 << 0xf;
            break;
        }
    }
}
