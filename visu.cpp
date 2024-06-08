#include <iostream>
#include <SFML/Graphics.hpp>
#include <set>
#include <tuple>
#include <type_traits>
#include "hex.h"
#include "visu.h"
#include "game.h"

const int LINE_SIZE = 40;
const int COLUMN_SIZE = 80;

const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = 1500;

const sf::Time  REFRESH_DELAY = sf::milliseconds(150);

sf::Vector2f operator+(const sf::Vector2f &a, const sf::Vector2f &b)
{
    return sf::Vector2f(a.x+b.x, a.y+b.y);
}

sf::Vector2f operator/(const sf::Vector2f &a, const float& b)
{
    return sf::Vector2f(a.x/b, a.y/b);
}

std::ostream& operator<<(std::ostream& os, const sf::Color& c)
{
    os << "sf::Color{"<<int{c.r}<<','<<int{c.g}<<','<<int{c.b}<<'}';
    return os;
}

char charTo16(char c)
{
    if (c >= '0' && c<='9')
    {
        return static_cast<char>(c-'0');
    }
    else if(c >= 'a' && c <= 'f')
    {
        return static_cast<char>(c-'a')+10;
    }
    else if(c>= 'A' && c <= 'F')
    {
        return static_cast<char>(c-'A')+10;
    }
    else
    {
        std::cerr<<"Character must be inside [0-9a-zA-Z]";
        return 0;
    }
}

float getIntermediateValue(float beginValue, float endValue, float ratio)
{
    return beginValue*(ratio) + endValue*(1-ratio);
}

sf::Color hexToColor(std::string hexColor)
{
    sf::Color res = sf::Color::Red;

    if (hexColor.size() != 6)
    {
        std::cerr << "Wrong color : ["<<hexColor<<']'<<std::endl;
        return sf::Color::Red;
    }

    sf::Uint8 r = 16*charTo16(hexColor[0])+charTo16(hexColor[1]);
    sf::Uint8 g = 16*charTo16(hexColor[2])+charTo16(hexColor[3]);
    sf::Uint8 b = 16*charTo16(hexColor[4])+charTo16(hexColor[5]);

    res.r = r;
    res.g = g;
    res.b = b;
    return res;
}

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

std::vector<sf::Vector2f> getPoints()
{
    std::vector<sf::Vector2f> res(13);
    res[0] =  {COLUMN_SIZE,           0};
    res[1] =  {2*COLUMN_SIZE,   LINE_SIZE};
    res[2] =  {2*COLUMN_SIZE, 3*LINE_SIZE};
    res[3] =  {  COLUMN_SIZE, 4*LINE_SIZE};
    res[4] =  {            0, 3*LINE_SIZE};
    res[5] =  {            0,   LINE_SIZE};
    res[6] =  {1*COLUMN_SIZE, 2*LINE_SIZE};
    res[7] =  (res[0]+res[1]+res[6])/3.f;
    res[8] =  (res[1]+res[2]+res[6])/3.f;
    res[9] =  (res[2]+res[3]+res[6])/3.f;
    res[10] = (res[3]+res[4]+res[6])/3.f;
    res[11] = (res[4]+res[5]+res[6])/3.f;
    res[12] = (res[5]+res[0]+res[6])/3.f;
    return res;
}

std::vector<sf::Vector2f> getPoints(float tau)
{
    std::vector<sf::Vector2f> res(13);
    res[0] =  {COLUMN_SIZE,           0};
    res[1] =  {getIntermediateValue(COLUMN_SIZE, 2*COLUMN_SIZE, tau),   LINE_SIZE};
    res[2] =  {getIntermediateValue(COLUMN_SIZE, 2*COLUMN_SIZE, tau), 3*LINE_SIZE};
    res[3] =  {  COLUMN_SIZE, 4*LINE_SIZE};
    res[4] =  {getIntermediateValue(COLUMN_SIZE, 0, tau), 3*LINE_SIZE};
    res[5] =  {getIntermediateValue(COLUMN_SIZE, 0, tau),   LINE_SIZE};
    res[6] =  {COLUMN_SIZE, 2*LINE_SIZE};
    res[7] =  (res[0]+res[1]+res[6])/3.f;
    res[8] =  (res[1]+res[2]+res[6])/3.f;
    res[9] =  (res[2]+res[3]+res[6])/3.f;
    res[10] = (res[3]+res[4]+res[6])/3.f;
    res[11] = (res[4]+res[5]+res[6])/3.f;
    res[12] = (res[5]+res[0]+res[6])/3.f;
    return res;
}

sf::ConvexShape createTriakis(std::tuple<int, int,int> point_nbs, std::tuple<float, float> position, const std::vector<sf::Vector2f>& points)
{
    sf::ConvexShape t;
    t.setPointCount(3);
    t.setPoint(0, points[std::get<0>(point_nbs)]);
    t.setPoint(1, points[std::get<1>(point_nbs)]);
    t.setPoint(2, points[std::get<2>(point_nbs)]);
    t.setOutlineThickness(1.f);
    t.setOutlineColor(sf::Color::White);
    t.setFillColor(sf::Color::Black);
    t.setPosition(std::get<0>(position), std::get<1>(position));
    return t;
}

void drawTriakis(sf::RenderWindow& window, float tau)
{
    std::vector<sf::Vector2f> points = getPoints(tau);
    int li = -3;
    int col;
    while (li*LINE_SIZE < WINDOW_HEIGHT)
    {
        col = (li%2) ? 0 : -1;
        while (col*COLUMN_SIZE < WINDOW_WIDTH)
        {
            std::vector<std::tuple<int,int,int>> triplets = {{0,7,1}, {1,7,6}, {6,7,0},
                                                            {1,8,2}, {2,8,6}, {6,8,1},
                                                            {2,9,3}, {3,9,6}, {6,9,2},
                                                            {3,10,4}, {4,10,6}, {6,10,3},
                                                            {4, 11, 5}, {5,11,6}, {6,11,4},
                                                            {5,12,0}, {0,12,6}, {6,12,5}};

            // std::vector<sf::Color> colors = {
            //                                 hexToColor("8F2AEB"), hexToColor("332C31"), hexToColor("302C33"),
            //                                 hexToColor("E3B7EB"), hexToColor("6B494B"), hexToColor("50496B"),
            //                                 hexToColor("6B4960"), hexToColor("5B496B"), hexToColor("DC88EB"),
            //                                 hexToColor("AB4951"), hexToColor("5C49AB"), hexToColor("5C49AB"),
            //                                 hexToColor("7C49AB"), hexToColor("EB2AAC"), hexToColor("CE2AEB"),
            //                                 hexToColor("502AEB"), hexToColor("EB2A39"), hexToColor("D559EB")};

            //std::vector<sf::Color> colors = {hexToColor("D19392"),hexToColor("D14D4B"), hexToColor("EB0B07"),
            //                                hexToColor("93B7D1"), hexToColor("366D96"), hexToColor("078BEB"),
            //                                hexToColor("6B6B39"), hexToColor("C8C838"), hexToColor("EBEB07"),
            //                                hexToColor("B49779"), hexToColor("966332"), hexToColor("EB7500"),
            //                                hexToColor("A18CD1"), hexToColor("5F3BB4"), hexToColor("4500EB"),
            //                                hexToColor("70E07D"), hexToColor("2BAB3A"), hexToColor("00EB1B")};

            std::vector<sf::Color> colors = {hexToColor("0013E6"),hexToColor("28293C"), hexToColor("303891"),
                                            hexToColor("307D91"), hexToColor("27373B"), hexToColor("00B7E6"),
                                            hexToColor("309132"), hexToColor("283C28"), hexToColor("00E604"),
                                            hexToColor("917F30"), hexToColor("3C3828"), hexToColor("E6BA00"),
                                            hexToColor("914830"), hexToColor("3C2D28"), hexToColor("E53800"),
                                            hexToColor("913087"), hexToColor("3C283A"), hexToColor("E600CE")};

            // for (std::tuple<int, int,int> triplet : triplets)
            std::vector<sf::Color>::iterator it_colors = colors.begin();
            std::vector<std::tuple<int, int, int>>::iterator it_triplets = triplets.begin();
            for (; it_triplets != triplets.end(); it_triplets++, it_colors++)
            {
                auto t = createTriakis(*it_triplets, {col*COLUMN_SIZE, li*LINE_SIZE}, points);
                t.setFillColor(*it_colors);
                window.draw(t);
            }
            col+=2;
        }
        li+=3;
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

            std::pair<int, int> hexCoords{li/3, (3*col-li)/6};
            hex.setFillColor((whiteTiles.find(hexCoords) != whiteTiles.end()) ? sf::Color::White : sf::Color::Black);
            hex.setOutlineColor((whiteTiles.find(hexCoords) != whiteTiles.end()) ? sf::Color::Black : sf::Color::White);

            window.draw(hex);
            col+=2;
        }
        li+=3;
    }
}

void drawTest()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

    std::cout << "Colour 0f0f0f " << hexToColor("0f0f0f")<<std::endl;
    std::cout << "Colour f0f0f0 " << hexToColor("f0f0f0")<<std::endl;

    std::cout << "Draw Test"<<std::endl;

    float tau = 0;
    float deltaTau = 0.008;

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
        
        if ((tau < 1 && tau+deltaTau>=1) || (tau >= 0 && tau+deltaTau<0))
        {
            deltaTau *= -1;
        }
        tau += deltaTau;
        // drawGrid(window);
        drawTriakis(window, tau);
        // window.draw(line, 2, sf::Lines);
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
    sf::Clock clock;

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
        if (clock.getElapsedTime() > REFRESH_DELAY)
        {
            clock.restart();
            window.clear();

            // drawGrid(window);
            drawColourHex(offsetTiles, window);
            window.display();
            offsetTiles = getNextStep(offsetTiles);
        }
    }
}

void draw(std::set<std::pair<int,int>> tiles)
{
    // mainDraw(tiles);
    drawTest();
    return;
}