// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

#include "graphicsUtils.hpp"
#include "sorts.hpp"
#include "sortstat.hpp"

void benchmarkSort(AnnotatedSort<SortingStat<int>> &sort);

int main() {
    BubbleSort<SortingStat<int>> bs;
    SelectionSort<SortingStat<int>> ss;
    InsertionSort<SortingStat<int>> is;
    QuickSort<SortingStat<int>> qs;

    const unsigned int SORTS_NUM = 4;

    AnnotatedSort<SortingStat<int>> *sort_ptrs[] = {&bs, &ss, &is, &qs};

    Button sortButtons[SORTS_NUM];

    sf::RenderWindow mainWindow(sf::VideoMode(1200, 700), "Sorting algorithm analyzer", sf::Style::None);
    mainWindow.clear();
    mainWindow.display();

    sf::Font font;

    font.loadFromFile("HelveticaMedium.ttf");

    MouseInteractiveEH eventHandler;

    for (int i = 0; i < SORTS_NUM; i++) {
        sortButtons[i].setFont(font);
        sortButtons[i].setFontColor(sf::Color::White);
        sortButtons[i].setBkgColor(sf::Color::Black);
        sortButtons[i].setHoverBkgColor(sf::Color(100, 100, 100));
        sortButtons[i].setClickBkgColor(sf::Color::White);
        sortButtons[i].setClickFontColor(sf::Color::Black);
        sortButtons[i].setOutlineThickness(2);
        sortButtons[i].setOutlineColor(sf::Color::White);
        sortButtons[i].setCharacterSize(30);
        sortButtons[i].setSize(sf::Vector2f(220, 80));
        sortButtons[i].setPosition(100 + 260 * i, 550);
        sortButtons[i].setString(sort_ptrs[i]->getName());
        sortButtons[i].setCallback([&sort_ptrs, i]() { benchmarkSort(*sort_ptrs[i]); });
        eventHandler.attach(sortButtons[i]);
    }

    // Button myButton;
    // myButton.setFont(font);
    // myButton.setFontColor(sf::Color::White);
    // myButton.setBkgColor(sf::Color::Black);
    // myButton.setHoverBkgColor(sf::Color(100, 100, 100));
    // myButton.setClickBkgColor(sf::Color::White);
    // myButton.setClickFontColor(sf::Color::Black);
    // myButton.setOutlineThickness(2);
    // myButton.setOutlineColor(sf::Color::White);
    // myButton.setCharacterSize(30);
    // myButton.setSize(sf::Vector2f(220, 80));
    // myButton.setPosition(100, 100);
    // myButton.setString("My button");

    // Button myButton2 = myButton;

    // myButton2.setString("My button 2");
    // myButton2.setPosition(360, 100);

    // eventHandler.attach(myButton);
    // eventHandler.attach(myButton2);

    // myButton.setCallback([]() { std::cout << "Нажата кнопка 1" << std::endl; });
    // myButton2.setCallback([]() { std::cout << "Нажата кнопка 2" << std::endl; });

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
        for (int i = 0; i < SORTS_NUM; i++) {
            mainWindow.draw(sortButtons[i]);
        }
        mainWindow.display();
    }

    return 0;
}

template <typename T>
void shuffle(T *array, size_t size) {
    if (size > 1) {
        for (size_t i = 0; i < size; i++) {
            size_t j = i + rand() / (RAND_MAX / (size - i) + 1);
            std::swap(array[i], array[j]);
        }
    }
}

void benchmarkSort(AnnotatedSort<SortingStat<int>> &sort) {
    size_t length = strlen(sort.getName());

    printf("|%s|\n", "==========================================");
    printf("| %*s%*c |\n", ((40 - length) >> 1) + length, sort.getName(), ((40 - length) >> 1) + ((40 - length) & 1), ' ');
    printf("|%s|\n", "==========================================");
    printf("| Array length | Comparisons | Assignments |\n");
    printf("|%s|\n", "==========================================");

    for (size_t i = 10; i <= 100; i += 10) {
        SortingStat<int> *arr = new SortingStat<int>[i];

        for (size_t j = 0; j < i; j++) {
            arr[j] = j;
        }

        shuffle(arr, i);

        SortingStat<int>::resetCounters();

        sort(arr, i);

        printf("| %12.lu | %11.llu | %11.llu |\n", i, SortingStat<int>::getComparisonCount(), SortingStat<int>::getAssignmentCount());
    }

    printf("|%s|\n", "==========================================");
}