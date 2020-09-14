// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SFML/Graphics.hpp>
#include <iostream>

#include "sorts.hpp"
#include "sortstat.hpp"
#include "graphicsUtils.hpp"

int main() {
    sf::RenderWindow mainWindow(sf::VideoMode(1200, 700), "Sorting algorithm analyzer", sf::Style::None);
    mainWindow.clear();
    mainWindow.display();

    sf::Font font;

    font.loadFromFile("HelveticaMedium.ttf");

    Button myButton;
    myButton.setFont(font);
    myButton.setFontColor(sf::Color::White);
    myButton.setBkgColor(sf::Color::Black);
    myButton.setHoverBkgColor(sf::Color(100, 100, 100));
    myButton.setClickBkgColor(sf::Color::White);
    myButton.setClickFontColor(sf::Color::Black);
    myButton.setOutlineThickness(2);
    myButton.setOutlineColor(sf::Color::White);
    myButton.setCharacterSize(30);
    myButton.setSize(sf::Vector2f(220, 80));
    myButton.setPosition(100, 100);
    myButton.setString("My button");

    Button myButton2 = myButton;

    myButton2.setString("My button 2");
    myButton2.setPosition(360, 100);

    MouseInteractiveEH eventHandler;

    eventHandler.attach(myButton);
    eventHandler.attach(myButton2);

    while (mainWindow.isOpen()) {
        sf::Event event;
        
        while (mainWindow.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    mainWindow.close();
                    break;

                default:
                    break;
            }
            eventHandler.handleEvent(event);
        }
        mainWindow.draw(myButton);
        mainWindow.draw(myButton2);
        mainWindow.display();
    }

    return 0;
}