#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class AnimatedAssets: public sf::Sprite
{
private:
    float Position_x = 0.f;
    float Speed_x = 0.f;
    float bound_right = 0.f;
public:
    // animated background
    AnimatedAssets(const float x, const float speed, sf::Texture *texture): Position_x(x), Speed_x(speed)
    {
        setTexture(*texture);
        texture->setRepeated(true);
        setTextureRect(sf::IntRect(0, 0, 900, 504));
        setPosition(Position_x,0);
    }

    void animate()
    {
        auto bound = getGlobalBounds();
        bound_right = bound.left + bound.width;

        if(bound_right <= 0)
        {
            move(1800,0);
        }
        move(Speed_x,0);
    }

public:
    float TopScr,BttScr,LftScr,RgtScr,t_,b_,l_,r_;
    sf::Vector2f Speed_;        //ALL OBJECTS USE THIS SPEED

    AnimatedAssets(sf::Vector2f size, sf::Vector2f position, sf::Vector2f v)
    {
        scale(size);
        setPosition(position);
        Speed_=v;

    }

    void ContinousAnimation(const sf::Time &elapsed, const sf::Vector2f &speed)
    {

        move(speed.x*elapsed.asSeconds(),speed.y*elapsed.asSeconds());  //we apply the movement
    }
};
class Bird: public AnimatedAssets
{
public:
    Bird(sf::Vector2f pos, sf::Vector2f speed):
        AnimatedAssets(sf::Vector2f(0.1,0.1), pos, speed){}

    void Gravity(sf::Time elapsed)
    {
        Speed_.y+=800*elapsed.asSeconds();
    }
    void stop_gravity()
    {
        if(Speed_.y > 0)
            Speed_.y = 0;
    }
};

class Pipe: public sf::Sprite
{
public:
    Pipe(const float x, const float y , const float speed, sf::Texture *texture): Position_x(x), Position_y(y), Speed_x(speed)
    {
        setTexture(*texture);        
        setTextureRect(sf::IntRect(0,-400, 280, 2000));
        setPosition(Position_x,Position_y - 150);
        setScale(0.4f,0.35f);
    }
    void animate(float Time)
    {
        /* moves pipe to the left, and if pipes right bound is smaller than windows left bound,
        pipe is moved, so that its left bound is bigger than windows right bound, so it will be animated again*/

        auto bound = getGlobalBounds();
        bound_right = bound.left + bound.width;
        if(bound_right + 210 <= 0)
        {
            crossed_changer();
            move(1210,0);
        }
        move(Speed_x*Time,0);
    }

    void crossed_changer()
    {
        crossed = !crossed;
    }

    bool crossed_return()
    {
        return crossed;
    }

private:
    float Position_x = 0.f;
    float Position_y = 0.f;
    float Speed_x = 0.f;
    float time;

    float bound_top = 0.f;
    float bound_bottom = 0.f;
    float bound_left = 0.f;
    float bound_right = 0.f;

    bool crossed = false;
};

class Start: public sf::Sprite
{
public:
    Start(sf::Texture *texture)
    {
        setTexture(*texture);
        setPosition(300,125);
        setScale(0.45f,0.45f);
    }
private:
};

void random_at_start(std::vector<Pipe> &vec)
{
    int a = rand() % 240 - 120;
    auto pos_0 = vec[0].getPosition();
    auto pos_1 = vec[0].getPosition();
        vec[0].setPosition(pos_0.x,- 180 + a);
        vec[1].setPosition(pos_1.x,180 + a);
}

void random(std::vector<Pipe> &vec)
{
    int a = rand() % 260 - 130;
    auto top = vec[0].getGlobalBounds();
    auto pos_0 = vec[0].getPosition();
    auto pos_1 = vec[0].getPosition();
    if(top.left + top.width + 210 <= 0)
    {
        vec[0].setPosition(pos_0.x,- 180 + a);
        vec[1].setPosition(pos_1.x,180 + a);
    }
}

void Points(int *point,std::vector<std::vector<Pipe>> &all_pipes)
{
    for(auto &i: all_pipes)
    {
        for(auto &a: i)
        {
            sf::FloatRect a_bounds = a.getGlobalBounds();
            if(a_bounds.left + a_bounds.width < 100 && a.crossed_return() == false)
            {
                *point = *point + 1;
                a.crossed_changer();
                std::cout << *point << std::endl;
            }
            else
            {
            }
        }
    }
}

int main()
{
    float dt = 0;
    bool space_clicked = false;
    std::string points_s = "0";
    int point_i = 0;

    std::vector<std::unique_ptr<AnimatedAssets>> spritesToDraw;
    sf::RenderWindow window(sf::VideoMode(900, 504), "Flappy bird");

    sf::Texture texture_background;
    if(!texture_background.loadFromFile("background.png")) { return 1; };
    AnimatedAssets background_1(0,-0.02,&texture_background);
    AnimatedAssets background_2(900,-0.02,&texture_background);

    std::vector<AnimatedAssets> backgrounds;
    backgrounds.emplace_back(background_1);
    backgrounds.emplace_back(background_2);
    sf::Clock clock;

    //creating background

    sf::Texture back_tex;
    if (!back_tex.loadFromFile("background.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    sf::Sprite background;
    background.setTexture(back_tex);
    float scalex = (float) window.getSize().x/back_tex.getSize().x;
    float scaley =(float) window.getSize().y/back_tex.getSize().y;
    background.setScale(scalex, scaley);

    //PLAYER
    Bird player(sf::Vector2f(100,window.getSize().y/2),sf::Vector2f(0,0));
    sf::Texture bird;
    if (!bird.loadFromFile("bird.png"))
        std::cerr << "Could not load texture" << std::endl;
    player.setTexture(bird);

    sf::Sprite birds;
    player.setPosition(100,200);

    //loading pipes
    sf::Texture pip;
    if (!pip.loadFromFile("pipe_bot.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    //vector that will contain all the pipes
    std::vector<sf::Sprite>pipes;

    //creating pipes
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

    for(auto &i: all_pipes)
    {
        random_at_start(i);
    }

    sf::Texture texture_start;
    if(!texture_start.loadFromFile("start.png")) { return 1; };
    Start start(&texture_start);

    //creating font
    sf::Font MyFont;
    if (!MyFont.loadFromFile("pixel_font.ttf"))
    {
        std::cout << "nie działa :(";
    }

    sf::Text text;
    text.setFont(MyFont);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::Black);
    text.setScale(2.f, 2.f);
    text.move(20.f, 0.f);
    text.setString(points_s);

    while (window.isOpen())

    {

        Points(&point_i,all_pipes);
        sf::Time elapsed = clock.restart();
        float time = elapsed.asSeconds();
        points_s = std::to_string(point_i/2);


        for(auto &s : spritesToDraw)
            s->ContinousAnimation(elapsed,s->Speed_);

        if(space_clicked == false)
        {

            player.move(0,0);
        }
        else
        {
//            dt = dt + time;
//            point_i = dt/2.f - 2;
            player.move(0,player.Speed_.y*elapsed.asSeconds());
            player.Gravity(elapsed);
            if(point_i < 0)
            {
                text.setString("0");
            }
            else
            {
                text.setString(points_s);
            }
        }


        sf::Event event;
        while (window.pollEvent(event))
        {

            /// "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // close game when esc is pressed

            if(event.type==sf::Event::KeyReleased)
            {
                if(event.key.code==sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
                if(event.key.code==sf::Keyboard::Key::Space)
                {
                    space_clicked = true;
                    player.stop_gravity();
                    player.Speed_+=sf::Vector2f(0,-400);
                }
            }
            // setting the mouse left click as the jumping button

            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    player.Speed_+=sf::Vector2f(0,-250);
                }
            }
        }
        // drawing stuff
        window.clear(sf::Color::Black);
        for(auto &i: backgrounds)
        {
            i.animate();
            window.draw(i);
        }

        if(space_clicked == false)
        {
            window.draw(start);
        }
        else
        {
            for(auto &i: all_pipes)
            {
                random(i);
                for(auto &a: i)
                {
                    a.animate(time);
                    window.draw(a);
                }
            }
        }
        window.draw(player);
        window.draw(text);
        window.display();
    }

    window.clear(sf::Color::Black);

    return 0;
}
