#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>


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
        AnimatedAssets(sf::Vector2f(1,1), pos, speed)
    {
        setScale(2,2);
    }
    void Set_Position(int pos_x, int pos_y)
    {
        setPosition(pos_x,pos_y);
    }
    void Gravity(float time)    //// Gravity
    {
        Speed_.y+=800*time;
    }
    void stop_gravity()
    {
        if(Speed_.y > 0)
            Speed_.y = 0;
    }
    void Falling(float rot,float time)
    {
        dt = dt + time;
        Gravity(time);
        setRotation(rot*dt);
    }
    void Dt_reset()
    {
        dt = 0;
    }
private:
    float dt = 0;
};

class Pipe: public sf::Sprite
{
public:
    Pipe(const float x, const float y , const float speed, sf::Texture *texture): Position_x(x), Position_y(y), Speed_x(speed)
    {
        setTexture(*texture);
        setPosition(Position_x,Position_y);
        setScale(0.4f,0.35f);
    }
    void animate(float Time, bool mode)
    {
        /* moves pipe to the left, and if pipes right bound is smaller than windows left bound,
        pipe is moved, so that its left bound is bigger than windows right bound, so it will be animated again*/
        auto bound = getGlobalBounds();
        bound_right = bound.left + bound.width;
        bound_bottom = bound_top + bound_bottom;
        if(bound_right + 210 <= 0)
        {
            crossed = false;
            move(1210,0);
        }
        if(mode == true)
        {
            move(Speed_x*Time,Speed_y*Time);
        }
        else
        {
            move(Speed_x*Time,0);
        }


    }
    void Set_crossed_false()
    {
        crossed = false;
    }
    bool crossed_return()
    {
        return crossed;
    }
    void Set_crossed_true()
    {
        crossed = true;
    }
    void Move_back()
    {
        auto pos = getPosition();
        setPosition(Position_x ,pos.y);
    }
    void Speed_y_()
    {
        Speed_y = Speed_y * -1;
    }

private:
    float Position_x = 0.f;
    float Position_y = 0.f;
    float Speed_x = 0.f;
    float Speed_y = 30.f;
    float time;

    float bound_top = 0.f;
    float bound_bottom = 0.f;
    float bound_left = 0.f;
    float bound_right = 0.f;

    bool crossed = false;
    bool x = false;
};

class Start_End: public sf::Sprite
{
public:
    Start_End(sf::Texture *texture,float x_pos, float y_pos, float x_scale, float y_scale):X_pos(x_pos), Y_pos(y_pos), X_scale(x_scale),Y_scale(y_scale)
    {
        setTexture(*texture);
        setPosition(X_pos,Y_pos);
        setScale(X_scale,Y_scale);
    }
    bool isClicked(sf::Vector2i &mouse_position) const
    {
        bool a = false;
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if(rectangle_bounds.top < mouse_position.y && mouse_position.y < rectangle_bounds.top + rectangle_bounds.height
            && rectangle_bounds.left < mouse_position.x && mouse_position.x < rectangle_bounds.left + rectangle_bounds.width)
        {
            a = true;
        }
        return (a);
    }
private:
    float X_pos = 0;
    float Y_pos = 0;
    float X_scale = 0;
    float Y_scale = 0;
};
//randomises localisation of gaps between pipes at start
void random_at_start(std::vector<Pipe> &vec)
{
    int a = rand() % 249 - 124;
    auto pos_0 = vec[0].getPosition();
    auto pos_1 = vec[0].getPosition();
    vec[0].setPosition(pos_0.x,-330+ a);
    vec[1].setPosition(pos_1.x,330+ a);
}
void Random_Speed_y_(std::vector<Pipe> &com, bool mode)
{
    if(mode == true)
    {
        int a = rand() % 2 - 1;
        std::cout << a << std::endl;
        if(a < 0)
        {
            com[0].Speed_y_();
            com[1].Speed_y_();
        }
    }
}
//randomises localisation of gaps between pipes after every relocation to the right
void random(std::vector<Pipe> &vec, bool mode)
{
    int a = rand() % 249 - 124;
    auto top = vec[0].getGlobalBounds();
    auto pos_0 = vec[0].getPosition();
    auto pos_1 = vec[0].getPosition();
    if(top.left + top.width + 210 <= 0)
    {
        vec[0].setPosition(pos_0.x,-330 + a);
        vec[1].setPosition(pos_1.x,330 + a);
        Random_Speed_y_(vec,mode);
    }
}
// updates point_i variable from main after pipe crosses player
void Points(int *point,std::vector<std::vector<Pipe>> &all_pipes)
{
    for(auto &i: all_pipes)
    {
        for(auto &a: i)
        {
            sf::FloatRect a_bounds = a.getGlobalBounds();
            if(a_bounds.left + a_bounds.width < 100.0 && a.crossed_return() == false)
            {
                std::cout << "works 3" << std::endl;
                *point = *point + 1;
                std::cout << *point << std::endl;
                a.Set_crossed_true();
            }
        }
    }
}
void Set_y_speed(std::vector<Pipe> &com, bool mode)
{
    if(mode == true)
    {
        auto pipe_1 = com[0].getGlobalBounds();
        auto pipe_2 = com[1].getGlobalBounds();
        if(pipe_1.top + pipe_1.height <= 50 || pipe_2.top >= 454)
        {
            com[0].Speed_y_();
            com[1].Speed_y_();
        }
    }
}

// checks if player intersects with piepes
bool Intersectcion(Pipe pipe,Bird player)
{
    bool x = false;

    auto player_bounds = player.getGlobalBounds();
    auto pipe_bounds = pipe.getGlobalBounds();
    if(player_bounds.intersects(pipe_bounds))
    {
        x = true;
    }
    else if(player_bounds.top <= 0 || player_bounds.top + player_bounds.height >= 900)
    {
        x = true;
    }
    else
    {
        x = false;
    }
    return x;
}
class SoundManager {
public:
    SoundManager(){
        if (!sb_jump.loadFromFile("jumpingsound.wav")) {
            std::cout << "ERROR: sounds didn't load" << std::endl;
        } else {
            s_jump.setBuffer(sb_jump);
        }

        if (!sb_freefall.loadFromFile("losingsound.wav"))
            std::cout << "ERROR: sounds didn't load" << std::endl;
        else{
            s_freefall.setBuffer(sb_freefall);
        }
        if(!bcgm.openFromFile("file_example_OOG_1MG.ogg")){
            std::cout<<"ERROR: sounds didn't open"<<std::endl;
        }
    }
    void jump() {
        s_jump.play();
    }
    void freefall() {
        s_freefall.play();
    }
    void music(){
        bcgm.setVolume(30);
        bcgm.play();
    }
    bool playMusicForFiveSeconds(){       // if i want to play a music for 5 seconds
        sf::Music music;
        if (!music.openFromFile("mixkit-player-jumping-in-a-video-game-2043.wav"))
            return false;
        music.setVolume(50);
        music.play();
        sf::sleep(sf::seconds(0.3));
        return true;
        // since music is a local variable, it is destroyed here (thus stopping playback)
    }

private:
    sf::SoundBuffer sb_jump;
    sf::Sound s_jump;
    sf::SoundBuffer sb_freefall;
    sf::Sound s_freefall;
    sf::Music bcgm;

};
int main()
{
    bool hard_mode = false;
    bool space_clicked = false;
    bool lost = false;
    std::string points_s = "0";
    std::string high_s = "0";
    int high_i = 0;
    int point_i = 0;
    SoundManager sounds;
    std::vector<std::unique_ptr<AnimatedAssets>> spritesToDraw;
    sf::RenderWindow window(sf::VideoMode(900, 504), "Flappy bird");

    sf::Texture texture_background;
    if(!texture_background.loadFromFile("background.png")) { return 1; };
    AnimatedAssets background_1(0,-0.025,&texture_background);
    AnimatedAssets background_2(900,-0.025,&texture_background);

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
    if(!texture_pipe_top.loadFromFile("pipe_top_test.png")) { return 1; };
    Pipe pipe_top_1(900,-3000,-150,&texture_pipe_top);
    Pipe pipe_top_2(1200,-3000,-150,&texture_pipe_top);
    Pipe pipe_top_3(1500,-3000,-150,&texture_pipe_top);
    Pipe pipe_top_4(1800,-3000,-150,&texture_pipe_top);

    sf::Texture texture_pipe_bot;
    if(!texture_pipe_bot.loadFromFile("pipe_bot_test.png")) { return 1; };
    Pipe pipe_bot_1(900,1200,-150,&texture_pipe_bot);
    Pipe pipe_bot_2(1200,1200,-150,&texture_pipe_bot);
    Pipe pipe_bot_3(1500,1200,-150,&texture_pipe_bot);
    Pipe pipe_bot_4(1800,1200,-150,&texture_pipe_bot);

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

    std::vector<std::vector<Pipe>> all_pipes;   // vector that will contain all the pipes

    all_pipes.emplace_back(combined_1);
    all_pipes.emplace_back(combined_2);
    all_pipes.emplace_back(combined_3);
    all_pipes.emplace_back(combined_4);

    for(auto &i: all_pipes)
    {
        random_at_start(i);
    }

    //press space, restart and game over sprites
    sf::Texture texture_start;
    if(!texture_start.loadFromFile("start.png")) { return 1; };
    Start_End start(&texture_start,250,100,0.60,0.60);

    sf::Texture texture_end;
    if(!texture_end.loadFromFile("end.png")) { return 1; };
    Start_End end(&texture_end,200,50,0.8,0.8);

    sf::Texture texture_restart;
    if(!texture_restart.loadFromFile("restart.png")) { return 1; };
    Start_End restart(&texture_restart,195,350,0.49,0.4);

    sf::Texture texture_easy_clicked;
    if(!texture_easy_clicked.loadFromFile("easy_clicked.png")) { return 1; };
    Start_End easy_clicked(&texture_easy_clicked,50,50,2,2);

    sf::Texture texture_easy_not_clicked;
    if(!texture_easy_not_clicked.loadFromFile("easy_not_clicked.png")) { return 1; };
    Start_End easy_not_clicked(&texture_easy_not_clicked,50,50,2,2);

    sf::Texture texture_hard_clicked;
    if(!texture_hard_clicked.loadFromFile("hard_clicked.png")) { return 1; };
    Start_End hard_clicked(&texture_hard_clicked,50,110,2,2);

    sf::Texture texture_hard_not_clicked;
    if(!texture_hard_not_clicked.loadFromFile("hard_not_clicked.png")) { return 1; };
    Start_End hard_not_clicked(&texture_hard_not_clicked,50,110,2,2);

    //creating font
    sf::Font MyFont;
    if (!MyFont.loadFromFile("pixel_font.ttf"))
    {
        std::cout << "nie dzia??a :(";
    }
    //Point font
    sf::Text text;
    text.setFont(MyFont);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::Black);
    text.setScale(1.f, 1.f);
    text.move(20.f, 20.f);
    text.setString(points_s);

    sf::Text text_2;
    text_2.setFont(MyFont);
    text_2.setOutlineThickness(2);
    text_2.setOutlineColor(sf::Color::Black);
    text_2.setScale(1.f, 1.f);
    text_2.move(450.f, 20.f);
    text_2.setString("High score " + high_s);

    while (window.isOpen())
    {
        //Points counter
        sf::Time elapsed = clock.restart();
        float time = elapsed.asSeconds();

        sf::Vector2i position = sf::Mouse::getPosition(window);

        Points(&point_i,all_pipes);
        if(point_i % 2 != 0 && point_i != 1)
        {
            point_i = point_i + 1;
        }
        points_s = std::to_string(point_i/2);
        if(point_i > high_i && lost == false)
        {
            high_i = point_i;
        }
        high_s = std::to_string(high_i/2);

        for(auto &s : spritesToDraw)
            s->ContinousAnimation(elapsed,s->Speed_);

        if(space_clicked == false)    //not starting the game until you press space
        {
            sounds.music();
            player.move(0,0);
        }
        else if(lost == false)
        {
            player.move(0,player.Speed_.y*elapsed.asSeconds());
            player.Gravity(time);
            if(point_i < 0)
            {
                text.setString("0");
                text_2.setString("0");
            }
            if(point_i == 1)
            {
                text.setString("1");
            }
            else
            {
                text.setString(points_s);
                text_2.setString("High score " + high_s);
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
                if(event.key.code==sf::Keyboard::Key::Space && lost == false)
                {
                    //sound.play();
                    space_clicked = true;
                    player.stop_gravity();
                    player.Speed_+=sf::Vector2f(0,-350);
                }
            }
            // resets game state

            if(event.type == sf::Event::MouseButtonPressed)      //// Restarting the game by pressing on restart
            {
                if(event.mouseButton.button == sf::Mouse::Left && restart.isClicked(position) == true)
                {
                    lost = false;
                    space_clicked = false;
                    player.setPosition(100,200);
                    player.stop_gravity();
                    player.Falling(0,time);
                    player.Dt_reset();
                    point_i = 0;
                    for(auto &i: all_pipes)
                    {
                        for(auto &a: i)
                        {
                            a.Set_crossed_false();
                            a.Move_back();
                        }
                    }
                }
                else if(event.mouseButton.button == sf::Mouse::Left && easy_not_clicked.isClicked(position) == true)
                {
                    hard_mode = !hard_mode;
                }
                else if(event.mouseButton.button == sf::Mouse::Left && hard_not_clicked.isClicked(position) == true)
                {
                    hard_mode = !hard_mode;
                }
            }
        }
        // drawing stuff
        window.clear(sf::Color::Black);

        for(auto &i: backgrounds)       //// background animations
        {
            i.animate();
            window.draw(i);
        }
        if(space_clicked == false)   // drawing press space to start
        {
            window.draw(start);
            if(hard_mode == false)
            {
                window.draw(easy_clicked);
                window.draw(hard_not_clicked);
            }
            else
            {
                for(auto &i: all_pipes)
                {
                   Random_Speed_y_(i,hard_mode);
                }
                window.draw(easy_not_clicked);
                window.draw(hard_clicked);
            }
        }
        else if(lost == true)        //// options after losing
        {
            sounds.music();
            player.move(0,player.Speed_.y*elapsed.asSeconds());
            player.Falling(70,time);

            for(auto &i: all_pipes)
            {
                for(auto &a: i)
                {
                    a.Set_crossed_false();
                    window.draw(a);
                }
            }
            window.draw(restart);
            window.draw(end);
        }
        else
        {
            for(auto &i: all_pipes)      //// randomizing the pipes
            {
                Set_y_speed(i,time);
                random(i,hard_mode);
                for(auto &a: i)
                {
                    a.animate(time, hard_mode);
                    window.draw(a);
                }
            }
        }

        for(auto &i: all_pipes)        //// collision for pipes
        {
            for(auto &a: i)
            {
                if(Intersectcion(a,player) == true)
                {
                    lost = true;
                    sounds.freefall();
                }
            }
        }

        if(hard_mode == true)
        {
            all_pipes[1][0].Move_back();
            all_pipes[1][1].Move_back();
            all_pipes[3][0].Move_back();
            all_pipes[3][1].Move_back();
        }

        //sound effect when jumping
        if(event.key.code==sf::Keyboard::Key::Space&& lost == false)
        {
            sounds.jump();
        }
        window.draw(player);
        window.draw(text);
        window.draw(text_2);
        window.display();

    }
    return 0;
}
