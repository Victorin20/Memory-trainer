#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;
using namespace sf;

void pause()
{
    Clock clock;
    clock.restart();
    int time(0), time2(0);
    while (time + 1000 >= time2)time2 = clock.getElapsedTime().asMilliseconds();

}


int isHover(int table[], Vector2i& pos, const int& allSquares, const int& squaresH, const int& space, const int& squareSize)
{

    for (int k(0); k < allSquares; k++)
    {

        bool isX = pos.x >= k % squaresH * squareSize + k % squaresH * space && pos.x <= k % squaresH * squareSize + k % squaresH * space + squareSize;
        bool isY = pos.y >= k / squaresH * squareSize + k / squaresH * space && pos.y <= k / squaresH * squareSize + k / squaresH * space + squareSize + 60;

        if (isX && isY)return k;
    }
    return -1;
}

bool alredy(vector<int>& table, const int& search, const int& allSquares)
{
    for (int k(0); k < table.size(); k++)
    {
        if (table[k] == search)return true;
    }
    return false;
}

void copyArray(int table[], int tableCopy[], const int& size)
{
    for (int k(0); k < size; k++)
    {
        tableCopy[k] = table[k];
    }
}

void clearTable(int table[], const int& size)
{
    for (int k(0); k < size; k++)
    {
        table[k] = 0;
    }
}

int main()
{
    int space = 10;
    int squaresH = 8;
    int squaresV = 8;
    int allSquares = squaresV* squaresH;
    int squareSize(100);
    bool once = false;
    bool onceClear = false;
    bool copyOnce = false;

    unsigned int activePoints = 0;
    unsigned int points = 3;
    int score = 0;

    vector<int> moves;

    int width(squaresH* squareSize + (squaresH - 1) * space), heigth(squaresV * squareSize + (squaresV - 1) * space + 60);
    srand(time(NULL));
    RenderWindow* window;

    window = new RenderWindow(sf::VideoMode(width, heigth), "Memory trainer");
    

    int c = 0;
    Clock clock;

    Font font;
    font.loadFromFile("font/LCALLIG.TTF");

    Text score_text;
    score_text.setFont(font);
    score_text.setCharacterSize(30);
    score_text.setPosition(100, 10);

    RectangleShape* squares = new RectangleShape[allSquares];

    auto table = new int[allSquares];
    auto tableCopy = new int[allSquares];


    for (int k(0); k < allSquares; k++)
    {
        squares[k].setFillColor(Color::White);
        squares[k].setSize(Vector2f(100, 100));
        table[k] = 0;
        tableCopy[k] = 0;
    }

    for (int k(0); k < allSquares; k++)
    {
        squares[k].setPosition(k % squaresH * squareSize + space * (k % squaresH),
            k / squaresV * squareSize + space * (k / squaresV) + 60);
    }

  
    float timer = 0, delay(7.0f);
    while (window->isOpen())
    {
        Vector2i pos = Mouse::getPosition(*window);
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        float time = clock.getElapsedTime().asSeconds();
        timer += time;
        clock.restart();

        score_text.setString("Score : " + to_string(score));

        if (activePoints == points)
        {
            once = false;
            onceClear = false;
            copyOnce = false;            
            points++;
            delay += 0.5;
            activePoints = 0;
            clearTable(tableCopy, allSquares);
            clearTable(table, allSquares);
            while (moves.size() != 0)moves.pop_back();
        }
        if (!once)
        {
            for (unsigned int k(0); k < points; k++)
            {
                int number = rand() % allSquares;

                if (moves.size() > 0)
                {

                    while (alredy(moves, number, allSquares))
                    {
                        number = rand() % allSquares;

                    }
                }

                moves.push_back(number);
                table[number] = 1;
            }
            once = true;
        }
        if (!copyOnce) { copyArray(table, tableCopy, allSquares); copyOnce = true; }

        for (int k(0); k < allSquares; k++)
        {
            if (!table[k])squares[k].setFillColor(Color::White);
        }

        if (timer > delay)
        {
            clearTable(table, allSquares);

            timer = 0;
        }


        for (int k(0); k < allSquares; k++)
        {

            int position = isHover(table, pos, allSquares, squaresH, space, squareSize);
            if (position >= 0)squares[position].setFillColor(Color::Green);
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                while (Mouse::isButtonPressed(Mouse::Left)) {}
                if (!tableCopy[position])
                {
                    table[position] = 2;
                    score -= 100;
                }
                else
                {
                    table[position] = 1;
                    score += 50;
                    activePoints++;
                }

            }

        }


        window->clear();
        for (int k(0); k < allSquares; k++)
        {

            if (table[k])
            {
                squares[k].setFillColor(Color::Blue);
            }

            if (table[k] == 2)squares[k].setFillColor(Color::Red);
            window->draw(squares[k]);
        }
        window->draw(score_text);
        window->display();
    }

}