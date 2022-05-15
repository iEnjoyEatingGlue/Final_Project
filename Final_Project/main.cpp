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
        /* moves background to the left, and if backgrounds right bound is smaller than windows left bound,
        background is moved, so that its left bound is bigger than windows right bound, so it will be animated again*/
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
        setScale(0.4f,0.35f);
    }
    void animate(const sf::Time &elapsed)
    {
        /* moves pipe to the left, and if pipes right bound is smaller than windows left bound,
        pipe is moved, so that its left bound is bigger than windows right bound, so it will be animated again*/

        auto bound = getGlobalBounds();
        bound_right = bound.left + bound.width;
        if(bound_right + 210 <= 0)
        {
            move(1210,0);
        }
        Time = elapsed.asSeconds();
        move(Speed_x*Time,0);
    }
private:
    float Position_x = 0.f;
    float Position_y = 0.f;
    float Speed_x = 0.f;
    float Time = 0.f;

    float bound_top = 0.f;
    float bound_bottom = 0.f;
    float bound_left = 0.f;
    float bound_right = 0.f;
};

int main()
{
    //wofcjniasdjipovcnasdjkcns
    // hey
    sf::RenderWindow window(sf::VideoMode(900, 504), "My window");

    sf::Texture texture_background;
    if(!texture_background.loadFromFile("background.png")) { return 1; };
    Background background_1(0,-0.01,&texture_background);
    Background background_2(900,-0.01,&texture_background);

    std::vector<Background> backgrounds;
    backgrounds.emplace_back(background_1);
    backgrounds.emplace_back(background_2);

    sf::Texture texture_pipe_top;
    if(!texture_pipe_top.loadFromFile("pipe_top.png")) { return 1; };
    Pipe pipe_top_1(900,0,-150,&texture_pipe_top);
    Pipe pipe_top_2(1200,0,-150,&texture_pipe_top);
    Pipe pipe_top_3(1500,0,-150,&texture_pipe_top);
    Pipe pipe_top_4(1800,0,-150,&texture_pipe_top);

    sf::Texture texture_pipe_bot;
    if(!texture_pipe_bot.loadFromFile("pipe_bot.png")) { return 1; };
    Pipe pipe_bot_1(900,300,-150,&texture_pipe_bot);
    Pipe pipe_bot_2(1200,300,-150,&texture_pipe_bot);
    Pipe pipe_bot_3(1500,300,-150,&texture_pipe_bot);
    Pipe pipe_bot_4(1800,300,-150,&texture_pipe_bot);

    std::vector<Pipe> combined_1;
    std::vector<Pipe> combined_2;
    std::vector<Pipe> combined_3;
    std::vector<Pipe> combined_4;

    combined_1.emplace_back(pipe_top_1);
    combined_1.emplace_back(pipe_bot_1);

    combined_2.emplace_back(pipe_top_2);
    combined_2.emplace_back(pipe_bot_2);

    combined_3.emplace_back(pipe_top_3);
    combined_3.emplace_back(pipe_bot_3);

    combined_4.emplace_back(pipe_top_4);
    combined_4.emplace_back(pipe_bot_4);

    std::vector<std::vector<Pipe>> all_pipes;

    all_pipes.emplace_back(combined_1);
    all_pipes.emplace_back(combined_2);
    all_pipes.emplace_back(combined_3);
    all_pipes.emplace_back(combined_4);

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
        for(auto &i: all_pipes)
        {
            for(auto &a: i)
            {
                a.animate(elapsed);
                window.draw(a);
            }
        }

        window.display();
    }
}



