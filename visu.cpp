#include <iostream>
#include <SFML/Graphics.hpp>
#include <set>
#include "hex.h"
#include "visu.h"

const int LINE_SIZE = 5;
const int COLUMN_SIZE = 10;

const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = 1500;

void drawGrid(sf::RenderWindow& window)
{
    std::vector<sf::Vertex*> res;
    for (int i = 0; i*COLUMN_SIZE < WINDOW_WIDTH; i++)
    {   
        sf::Vertex col[2] = {   
                sf::Vertex(sf::Vector2f(float(1+i*COLUMN_SIZE), float(1)), sf::Color(100, 100, 100)),
                sf::Vertex(sf::Vector2f(float(1+i*COLUMN_SIZE), float(WINDOW_HEIGHT)), sf::Color(100, 100, 100))
            };
        window.draw(col, 2, sf::Lines);
    }
    for (int j = 0; j*LINE_SIZE  < WINDOW_HEIGHT; j++)
    {

        sf::Vertex li[2] = {   
                sf::Vertex(sf::Vector2f(float(1),              float(1+j*LINE_SIZE)), sf::Color(100, 100, 100)),
                sf::Vertex(sf::Vector2f(float(WINDOW_WIDTH),  float(1+j*LINE_SIZE)), sf::Color(100, 100, 100))
            };
        window.draw(li, 2, sf::Lines);
    }
}

void drawHex(sf::RenderWindow& window)
{
    
    int li = -3;
    int col; 
    while (li*LINE_SIZE < WINDOW_HEIGHT)
    {
        col = (li % 2) ? 0 : -1;
        while (col*COLUMN_SIZE < WINDOW_WIDTH)
        {
            sf::ConvexShape hex;
            hex.setPointCount(6);
            hex.setPoint(0, sf::Vector2f(  COLUMN_SIZE,           0));
            hex.setPoint(1, sf::Vector2f(2*COLUMN_SIZE,   LINE_SIZE));
            hex.setPoint(2, sf::Vector2f(2*COLUMN_SIZE, 3*LINE_SIZE));
            hex.setPoint(3, sf::Vector2f(  COLUMN_SIZE, 4*LINE_SIZE));
            hex.setPoint(4, sf::Vector2f(            0, 3*LINE_SIZE));
            hex.setPoint(5, sf::Vector2f(            0,   LINE_SIZE));
            hex.setPosition(col*COLUMN_SIZE, li*LINE_SIZE);
            hex.setOutlineThickness(1.f);
            hex.setOutlineColor(sf::Color::White);
            hex.setFillColor(sf::Color::Black);
            window.draw(hex);
            col+=2;
        }
        li+=3;
    }
}

void drawColourHex(std::set<std::pair<int, int>> whiteTiles, sf::RenderWindow& window)
{
    int li = -3;
    int col; 
    while (li*LINE_SIZE < WINDOW_HEIGHT)
    {
        col = (li % 2 == 0) ? 0 : -1;
        while (col*COLUMN_SIZE < WINDOW_WIDTH)
        {
            sf::ConvexShape hex;
            hex.setPointCount(6);
            hex.setPoint(0, sf::Vector2f(  COLUMN_SIZE,           0));
            hex.setPoint(1, sf::Vector2f(2*COLUMN_SIZE,   LINE_SIZE));
            hex.setPoint(2, sf::Vector2f(2*COLUMN_SIZE, 3*LINE_SIZE));
            hex.setPoint(3, sf::Vector2f(  COLUMN_SIZE, 4*LINE_SIZE));
            hex.setPoint(4, sf::Vector2f(            0, 3*LINE_SIZE));
            hex.setPoint(5, sf::Vector2f(            0,   LINE_SIZE));
            hex.setPosition(col*COLUMN_SIZE, li*LINE_SIZE);
            hex.setOutlineThickness(1.f);
            hex.setOutlineColor(sf::Color::White);
            
            std::pair<int, int> hexCoords{li/3, (3*col-li)/6};
            hex.setFillColor((whiteTiles.find(hexCoords) != whiteTiles.end()) ? sf::Color::White : sf::Color::Black);
            window.draw(hex);
            col+=2;
        }
        li+=3;
    }
}

void drawTest(std::set<std::pair<int,int>> tiles)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(10, 10)),
        sf::Vertex(sf::Vector2f(300, 150))
    };


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                std::cout << "Close !" << std::endl;
                return;
            }
        }
        window.clear();
        drawGrid(window);
        std::set<std::pair<int, int>> set = {{1,1}, {1,2}, {2,2}};
        drawColourHex(set, window);
        window.draw(line, 2, sf::Lines);
        window.display();

    }
}

void mainDraw(std::set<std::pair<int, int>> tiles)
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");


    std::set<std::pair<int, int>> offsetTiles;
    std::pair<int, int> offset{25, 25};
    for(const std::pair<int,int>& tile : tiles)
    {
        offsetTiles.insert(tile+offset);
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                std::cout << "Close !" << std::endl;
                return;
            }
        }
        window.clear();

        // drawGrid(window);
        drawColourHex(offsetTiles, window);
        window.display();

    }
}

void draw(std::set<std::pair<int,int>> tiles)
{   
    mainDraw(tiles);
    return;
}