#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Background: public sf::Sprite
{
public:
    Background(const float x, const float speed, sf::Texture *texture): Position_x(x), Speed_x(speed)
    {
        setTexture(*texture);
        texture->setRepeated(true);
        setTextureRect(sf::IntRect(0, 0, 900, 504));
        setPosition(Position_x,0);
    }

    void animate()
    {
        /* function moves background to the left, and if backgrounds right bound is smaller then windows left bound,
        background is moved, so that its left bound is beyond windows right bound, so it will be animated again*/
        auto bound = getGlobalBounds();
        bound_right = bound.left + bound.width;

        if(bound_right <= 0)
        {
            move(1800,0);
        }
        move(Speed_x,0);
    }
private:
    float Position_x = 0.f;
    float Speed_x = 0.f;
    float bound_right = 0.f;
};

class Bird: public sf::Sprite
{
public:
    Bird(const float);
private:
};

class Pipe: public sf::Sprite
{
public:
    Pipe(const float x, const float y , const float speed, sf::Texture *texture): Position_x(x), Position_y(y), Speed_x(speed)
    {
        setTexture(*texture);
        setPosition(Position_x,Position_y);
        setTextureRect(sf::IntRect(0, 0, 280, 2000));
        setScale(0.3f,0.3f);
        move(400,0);
    }
private:
    float Position_x = 0.f;
    float Position_y = 0.f;
    float Speed_x = 0.f;

    float bound_top = 0.f;
    float bound_bottom = 0.f;
    float bound_left = 0.f;
    float bound_right = 0.f;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(900, 504), "My window");

    sf::Texture texture_background;
    if(!texture_background.loadFromFile("background.png")) { return 1; };
    Background background_1(0,-0.01,&texture_background);
    Background background_2(900,-0.01,&texture_background);

    std::vector<Background> backgrounds;
    backgrounds.emplace_back(background_1);
    backgrounds.emplace_back(background_2);

    sf::Texture texture_pipe_bot;
    if(!texture_pipe_bot.loadFromFile("pipe_bot.png")) { return 1; };
    Pipe pipe_bot(0,0,0,&texture_pipe_bot);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear(sf::Color::Black);
        for(auto &i: backgrounds)
        {
            i.animate();
            window.draw(i);
        }
        window.draw(pipe_bot);
        window.display();
    }
}



