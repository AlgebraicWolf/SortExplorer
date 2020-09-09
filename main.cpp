// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SFML/Graphics.hpp>
#include <iostream>

#include "sorts.hpp"
#include "sortstat.hpp"

int main() {
    sf::RenderWindow mainWindow(sf::VideoMode(800, 600), "Sorting algorithm analyzer", sf::Style::None);
    mainWindow.clear();
    mainWindow.display();

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
        }
    }

    return 0;
}