#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#define startingPos 260
#define framerate 120.f
#define Pipe_Width 128
#define DefPipePos 800.f
void resetPipe(sf::RectangleShape &Top, sf::RectangleShape &Bottom, int X) {
    float Randomnum = std::rand() % 400 + 200;
    Top.setPosition(X, Randomnum-110);
    Bottom.setPosition(X, Randomnum+110);
}
bool isRectInBounds(int Px, int Py, int Bx, int By, int Bw, int Bh, bool upORdown) {
    if (upORdown) {
        if (Px <= Bx && Px + 128 >= Bx - Bw/2) {
            if (Py >= By && Py >= By - Bh / 2) {
                return true;
            }
        }
    }
    else {
        if (Px <= Bx && Px+128 >= Bx - Bw / 2) {
            if (Py <= By && Py <= By + Bh / 2) {
                return true;
            }
        }
    }
    return false; 
}

bool Scorecounter(int Px, int Py, int Bx, int By, int Bw, int Bh, int &score, bool &inScoreBound) {
    if (Px <= Bx && Px + 128 >= Bx - Bw / 2) {
        if (Py <= By && Py <= By + Bh / 2 && inScoreBound) {
            score += 1;
            inScoreBound = false;
        }        
    } else {
        inScoreBound = true;
        }
    return false;
}

bool Scorecounter(sf::Vector2i Pipe, sf::Vector2i Bird, sf::Vector2i Birdsize, int &score, bool &inScoreBound) {
    return Scorecounter(Pipe.x, Pipe.y, Bird.x, Bird.y, Birdsize.x, Birdsize.y, score, inScoreBound);
}

bool isRectInBounds(sf::Vector2i Pipe, sf::Vector2i Bird, sf::Vector2i Birdsize, bool upORdown) {
    return isRectInBounds(Pipe.x, Pipe.y, Bird.x, Bird.y, Birdsize.x, Birdsize.y,upORdown);
}

void DrawBackGround(sf::RectangleShape &Background, float &curPos,sf::RenderWindow &window) {
    for (int i = 0; i <= 1; i++) {

        if (i == 1) {
            Background.setPosition(curPos + 900, 0);
        }
        if (curPos <= -1000) {
            curPos = -100.0f;
            Background.setPosition(curPos, 0);
        }
        window.draw(Background);
        Background.setPosition(curPos, 0);
    }
}

struct wingTPicker {
    int TX = 0; 
    sf::IntRect rect;

    wingTPicker() {
        rect.height = 32;
        rect.width = 32;
        rect.top = 0;
        rect.left = 0;
    }


    void wingFlap() {
        if (TX != 2) {
            rect.left = 32 * TX;
        }
        TX++;
        if (TX >= 2) {
            TX = 0;
        }
    }
};

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(800, 800), "Flappy Bird Clone");
    sf::RectangleShape Bird(sf::Vector2f(128, 128));
    sf::RectangleShape Flap(sf::Vector2f(128,128));
    sf::RectangleShape PipeL1(sf::Vector2f(Pipe_Width, 512));
    sf::RectangleShape PipeT1(sf::Vector2f(Pipe_Width, 512));
    sf::RectangleShape PipeL2(sf::Vector2f(Pipe_Width, 512));
    sf::RectangleShape PipeT2(sf::Vector2f(Pipe_Width, 512));
    sf::RectangleShape BackGround(sf::Vector2f(1000, 800));
    sf::RectangleShape BackGround2(sf::Vector2f(1000, 800));
    sf::RectangleShape BackGround3(sf::Vector2f(1000, 800));
    sf::RectangleShape GameOver(sf::Vector2f(400, 400));

    sf::Vector2i Bird_Size(Bird.getSize());
    window.setFramerateLimit(framerate);
    Bird.setPosition(startingPos, startingPos);
    Bird.setOrigin(64, 64);
    Flap.setOrigin(64, 64);
    PipeT1.setOrigin(128, 0);
    PipeT2.setOrigin(128, 0);
    BackGround.setPosition(-100, 0);
    BackGround2.setPosition(-100, 0);
    BackGround3.setPosition(-100, 0);
    GameOver.setPosition(400, 400);
    GameOver.setOrigin(200, 200);
    GameOver.setFillColor(sf::Color(26, 23, 23));
    bool spacepush, spaceheld, DelayBool, GameOverB, restart, scoreCB1, scoreCB2;
    spacepush = spaceheld = DelayBool = GameOverB = restart = scoreCB1 = scoreCB2 = false;

    sf::Clock clock, wingTime, FPSclock, Delay;
    sf::Time time;
    sf::Text FCounter, gameText, gameScore, RText, SCORE;
    sf::Font FcounterFONT;
    std::ifstream File;

    int scoreC = 0;
    int bestscore = 0;
    float startingPosY = 400;
    int FPS = 0;
    float numForRotatione = 0;
    float Pipemove_X = 1600;
    float Pipemove_X2 = 2000;
    std::string FPSS,SCOREstring;
    float BackSpeed1, BackSpeed2, BackSpeed3;
    BackSpeed1 = BackSpeed2 = BackSpeed3 = -100;
    sf::Texture falppybird, flap, pipeTex, back1, back2, back3;


    if (!falppybird.loadFromFile("assets/flapy bird.png")) {
        std::cout << "failed to load flapy bird.png";
    }
    if (!flap.loadFromFile("assets/flap.png")) {
        std::cout << "failed to load flap.png";
    }
    if (!pipeTex.loadFromFile("assets/pipes.png")) {
        std::cout << "failed to load flap.png";
    }
    if (!back1.loadFromFile("assets/Background_draw1.png")) {
        std::cout << "failed to load Background_draw1";
    }
    if (!back2.loadFromFile("assets/Background_draw2.png")) {
        std::cout << "failed to load Background_draw2";
    }
    if (!back3.loadFromFile("assets/Background_draw3.png")) {
        std::cout << "failed to load Background_draw3";
    }
    if (!FcounterFONT.loadFromFile("assets/arial.ttf")) {
        std::cout << "failed to load arial.ttf";
    }
    FCounter.setFont(FcounterFONT);
    FCounter.setCharacterSize(50);
    FCounter.setFillColor(sf::Color::Green);
    gameText.setFont(FcounterFONT);
    gameText.setCharacterSize(30);
    gameText.setString("!GAME OVER!");
    gameText.setPosition(300, 220);
    gameScore.setFont(FcounterFONT);
    gameScore.setCharacterSize(30);
    gameScore.setPosition(300, 460);
    gameScore.setString("!BEST SCORE!");
    RText.setFont(FcounterFONT);
    RText.setCharacterSize(20);
    RText.setPosition(340, 400);
    RText.setString("R for Restart");
    SCORE.setFont(FcounterFONT);
    SCORE.setCharacterSize(30);
    SCORE.setPosition(390, 490);
    wingTPicker wing;
    Bird.setTexture(&falppybird);
    Flap.setTexture(&flap);
    PipeL1.setTexture(&pipeTex);
    PipeL2.setTexture(&pipeTex);
    PipeT1.setTexture(&pipeTex);
    PipeT2.setTexture(&pipeTex);
    Flap.setTextureRect(wing.rect);
    BackGround.setTexture(&back1);
    BackGround2.setTexture(&back2);
    BackGround3.setTexture(&back3);

    resetPipe(PipeT1, PipeL1,800);
    resetPipe(PipeT2, PipeL2, 1200);
    PipeT1.setRotation(180);
    PipeT2.setRotation(180);
    while (window.isOpen())
    {
        time = clock.restart();
        float Time = time.asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();  
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            spacepush = true;
        }
        else {
            spaceheld = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            restart = true;
        }
        else {
            restart = false;
        }

        // FPS counter
        FPS++;
        if (FPSclock.getElapsedTime() >= sf::milliseconds(1000)) {
            FPSS = std::to_string(FPS);
            FCounter.setString(FPSS);
            FPS = 0;
            FPSclock.restart();
        }
        


        // pipe movement from right to left
        Pipemove_X -= 160.f * Time;
        Pipemove_X2 -= 160.f * Time;
        PipeT1.setPosition(Pipemove_X, PipeT1.getPosition().y);
        PipeL1.setPosition(Pipemove_X, PipeL1.getPosition().y);
        PipeT2.setPosition(Pipemove_X2, PipeT2.getPosition().y);
        PipeL2.setPosition(Pipemove_X2, PipeL2.getPosition().y);
        //resets pipe Pos when out of frame
        if (PipeT1.getPosition().x <= -Pipe_Width) {
            resetPipe(PipeT1, PipeL1,800);
            Pipemove_X = 800;
        }
        if (PipeT2.getPosition().x <= -Pipe_Width) {
            resetPipe(PipeT2, PipeL2, 800);
            Pipemove_X2 = 800;
        }

        bool mouseClickedOnce = spacepush && !spaceheld;
        //jumping code
        if (mouseClickedOnce) {
            Bird.setPosition(startingPos, startingPosY -= 11000.0f * Time);
            //std::cout << startingPosY << std::endl;
            Flap.setPosition(startingPos, startingPosY);
            numForRotatione = 300;
            DelayBool = true;
            Delay.restart();
            spacepush = false;
            if (!spaceheld) {
                spaceheld = true;
            }
        } else {
            spacepush = false;
            Bird.setPosition(Bird.getPosition().x, startingPosY += 220.0f * Time);
            Flap.setPosition(Bird.getPosition().x, startingPosY);
        }

        sf::Vector2i PipePos1(PipeT1.getPosition());
        sf::Vector2i PipePos2(PipeT2.getPosition());
        sf::Vector2i PipePos3(PipeL1.getPosition());
        sf::Vector2i PipePos4(PipeL2.getPosition());
        sf::Vector2i BirdCord(Bird.getPosition());
        if (isRectInBounds(PipePos1, BirdCord, Bird_Size,true) || isRectInBounds(PipePos2, BirdCord, Bird_Size,true)|| isRectInBounds(PipePos3, BirdCord, Bird_Size, false) || isRectInBounds(PipePos4, BirdCord, Bird_Size, false)) {
            if (bestscore <= scoreC && !GameOverB) {
                bestscore = scoreC;
                SCOREstring = std::to_string(bestscore);
                SCORE.setString(SCOREstring);
            }
            GameOverB = true; 
        }

        if (restart&& GameOverB) {
            resetPipe(PipeT1, PipeL1, 800);
            resetPipe(PipeT2, PipeL2, 1200);
            Pipemove_X = 1600;
            Pipemove_X2 = 2000;
            GameOverB = false;
            startingPosY = 400;
            scoreC = 0;
        }

        Scorecounter(PipePos1, BirdCord, Bird_Size, scoreC, scoreCB1);
        Scorecounter(PipePos2, BirdCord, Bird_Size, scoreC, scoreCB2);

        // this code makes the wing flap
        if (wingTime.getElapsedTime() >= sf::milliseconds(200)) {
            wingTime.restart();
            wing.wingFlap();
            Flap.setTextureRect(wing.rect);
        }

        Bird.setRotation(numForRotatione);
        Flap.setRotation(numForRotatione);
        // bird rotate delay after space is pushed
        if (DelayBool) {
            if (Delay.getElapsedTime() >= sf::milliseconds(600)) {
                DelayBool = false;
            }
        } else {
            if (Bird.getRotation() >= 358) {
                numForRotatione = 0;
            }
            if (Bird.getRotation() >= 88 && Bird.getRotation() <= 91) {
                numForRotatione = 88;
                
            }
            numForRotatione += 120*Time;
        }
        BackSpeed1 += -60 * Time;
        BackSpeed2 += -20 * Time;
        BackSpeed3 += -40 * Time;

        window.clear();
        DrawBackGround(BackGround3, BackSpeed3, window);
        DrawBackGround(BackGround2, BackSpeed2, window);
        DrawBackGround(BackGround, BackSpeed1, window);
        window.draw(Bird);
        window.draw(Flap);
        window.draw(PipeL1);
        window.draw(PipeT1);
        window.draw(PipeL2);
        window.draw(PipeT2);
        window.draw(FCounter);
        if (GameOverB) {
            window.draw(GameOver);
            window.draw(gameText);
            window.draw(gameScore);
            window.draw(RText);
            window.draw(SCORE);
        }
        window.display();
    }
    
    return 0;
};