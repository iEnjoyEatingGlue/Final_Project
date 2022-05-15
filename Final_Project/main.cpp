
#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc
//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc
//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc
//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc
//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc
//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcnSLADJKCADkjc
//acabujhocb adhjcbnsdajkclbnsdJKCLBSDNcnklsbDCLJKSDncjkldSNCKJLsadcnkjldsACNSDAJKLcn


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


        Speed_.y+=400*elapsed.asSeconds();

    }


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
    std::vector<std::unique_ptr<AnimatedAssets>> spritesToDraw;
    sf::RenderWindow window(sf::VideoMode(900, 504), "My window");

    sf::Texture texture_background;
    if(!texture_background.loadFromFile("background.png")) { return 1; };
    AnimatedAssets background_1(0,-0.01,&texture_background);
    AnimatedAssets background_2(900,-0.01,&texture_background);

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



    sf:: Event event ;
    while (window.isOpen())

    {
        std::cout<<"hello world0"<<std::endl;
        sf::Time elapsed = clock.restart();

        for(auto &s : spritesToDraw)
            s->ContinousAnimation(elapsed,s->Speed_);

        player.move(0,player.Speed_.y*elapsed.asSeconds());
        player.Gravity(elapsed);

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
        for(auto &i: all_pipes)
        {
            for(auto &a: i)
            {
                a.animate(elapsed);
                window.draw(a);
            }
        }
        auto a = pipe_top_1.getGlobalBounds();



        window.draw(player);
        window.display();

    }

    window.clear(sf::Color::Black);

    return 0;
}
