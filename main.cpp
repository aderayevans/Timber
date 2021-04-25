#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
    /*
    //Create a video mode object
    VideoMode vm(1366, 768);

    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
    */

    // Low res code
    VideoMode vm(1280, 720);

    RenderWindow window(vm, "Timber!!!");
    View view(sf::FloatRect(0, 0, 1920, 1080));
    window.setView(view);
    // End of low res code!! Everything else is the same!!

    // Making background
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    // Making a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Enhanced version
    Texture textureTree2;
    textureTree2.loadFromFile("graphics/tree2.png");

    Sprite spriteTree2;
    spriteTree2.setTexture(textureTree2);
    Sprite spriteTree3;
    spriteTree3.setTexture(textureTree2);
    Sprite spriteTree4;
    spriteTree4.setTexture(textureTree2);
    Sprite spriteTree5;
    spriteTree5.setTexture(textureTree2);
    Sprite spriteTree6;
    spriteTree6.setTexture(textureTree2);

    spriteTree2.setPosition(20, 0);
    spriteTree3.setPosition(300, -400);
    spriteTree4.setPosition(1300, -400);
    spriteTree5.setPosition(1500, -500);
    spriteTree6.setPosition(1900, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    // Is this bee currently moving?
    bool beeActive = false;

    // How fast can bee fly?
    float beeSpeed = 0.0f;

    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    const int NUM_CLOUDS = 6;
    Sprite clouds[NUM_CLOUDS];
    int cloudSpeeds[NUM_CLOUDS];
    bool cloudsActive[NUM_CLOUDS];

    for (int i = 0; i < NUM_CLOUDS; i++)
    {
        clouds[i].setTexture(textureCloud);
        clouds[i].setPosition(-300, i * 150);
        cloudsActive[i] = false;
        cloudSpeeds[i] = 0;

    }

    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    // Draw some text
    int score = 0;
    Text messageText;
    Text scoreText;
    Text fpsText;

    // We need to choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);
    fpsText.setFont(font);

    // Assign actual text
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    fpsText.setCharacterSize(100);

    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    fpsText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin
    (
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
    );
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
    fpsText.setPosition(1200, 20);

    // Backgrounds for the text
    RectangleShape rect1;
    rect1.setFillColor(sf::Color(0, 0, 0, 150));
    rect1.setSize(Vector2f(600, 105));
    rect1.setPosition(0, 30);

    RectangleShape rect2;
    rect2.setFillColor(sf::Color(0, 0, 0, 150));
    rect2.setSize(Vector2f(1000, 105));
    rect2.setPosition(1150, 30);

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 10;//1000;
    float logSpeedY = 20;//-1500;

    // Control the player input
    bool acceptInput = false;

    // Prepare the sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    // control the drawing of the score
    int lastDrawn = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::F5)
                {
                    // Capturing the window to a sf::Image
                    sf::Vector2u windowSize = window.getSize();
                    sf::Texture texture;
                    texture.create(windowSize.x, windowSize.y);
                    texture.update(window);
                    sf::Image screenshot = texture.copyToImage();

                    // Making the screenshot name
                    std::string screenshotName = "screenshot";

                    // Saving to a file
                    screenshot.saveToFile("screenshots/" + screenshotName + ".png");
                }
            }
            if (event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;

                spriteAxe.setPosition(2000,
                    spriteAxe.getPosition().y);
            }
        }
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Start the game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            score = 0;
            timeRemaining = 6;

            // Make all branches disappear
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // Move the player into position
            spritePlayer.setPosition(580, 720);
            acceptInput = true;
        }
        // Wrap the player controls to
        // Make sure we are accepting input
        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT,
                    spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                chop.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT,
                    spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);

                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                chop.play();
            }
        }
        /*
        ****************************************
        Update the scene
        ****************************************
        */
        if (!paused)
        {
            // Measure time
            Time dt = clock.restart();

            // Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // Size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                paused = true;

                messageText.setString("Out of time!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin
                (
                    textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f
                );
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                outOfTime.play();
            }

            // Setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200; //random between 0+200 : 199+200

                // How high
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500; //random between 500 : 999
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y
                );
                // Has the bee reached the right hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }

            // Manage the clouds
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!cloudsActive[i])
                {
                    // How fast is the cloud
                    srand((int)time(0) * i);
                    cloudSpeeds[i] = (rand() % 200);

                    // How high is the cloud
                    srand((int)time(0) * i);
                    float height = (rand() % 150);
                    clouds[i].setPosition(-200, height);
                    cloudsActive[i] = true;

                }
                else
                {
                    // Set the new position
                    clouds[i].setPosition(
                        clouds[i].getPosition().x +
                        (cloudSpeeds[i] * dt.asSeconds()),
                        clouds[i].getPosition().y);

                    // Has the cloud reached the right hand edge of the screen?
                    if (clouds[i].getPosition().x > 1920)
                    {
                        // Set it up ready to be a whole new cloud next frame
                        cloudsActive[i] = false;
                    }
                }
            }

            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(0);
                }
                else
                {
                    branches[i].setPosition(3000, height);
                }
            }
            if (logActive)
            {
                spriteLog.setPosition(
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                // Has the insect reached the right hand edge of the screen?
                if (spriteLog.getPosition().x < -100 ||
                    spriteLog.getPosition().x > 2000)
                {
                    // Set it up ready to be a whole new cloud next frame
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }
            if (branchPositions[5] == playerSide)
            {
                // death
                paused = true;
                acceptInput = false;

                // Draw the gravestone
                spriteRIP.setPosition(525, 760);

                // Hide the player
                spritePlayer.setPosition(2000, 600);

                // Change the text of the message
                messageText.setString("SQUISHED!!");

                // Center the message on the screen
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(
                    textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f
                );

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                death.play();
            }

            // Draw the score and the frame rate once every 100 frames
            lastDrawn++;
            if (lastDrawn == 100 || branchPositions[5] == playerSide) {
                // Update the score text
                std::stringstream ss;
                ss << "Score = " << score;
                scoreText.setString(ss.str());

                // Draw the fps
                std::stringstream ss2;
                ss2 << "FPS = " << 1 / dt.asSeconds();
                fpsText.setString(ss2.str());
                lastDrawn = 0;
            }
        }
        /*
        ****************************************
        Draw the scene
        ****************************************
        */

        window.clear();

        window.draw(spriteBackground);

        for (int i = 0; i < NUM_CLOUDS; i++)
        {
            window.draw(clouds[i]);
        }

        window.draw(spriteTree2);
        window.draw(spriteTree3);
        window.draw(spriteTree4);
        window.draw(spriteTree5);
        window.draw(spriteTree6);

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spriteTree);

        window.draw(spritePlayer);

        window.draw(spriteAxe);

        window.draw(spriteLog);

        window.draw(spriteRIP);

        // Draw backgrounds for the text
        window.draw(rect1);
        window.draw(rect2);

        window.draw(spriteBee);

        window.draw(scoreText);

        // Draw the FPS
        window.draw(fpsText);

        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }
        window.display();
    }
    return 0;
}

// Function definition
void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    srand((int)time(0) + seed);
    int r = rand() % 5;
    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;

    case 1:
        branchPositions[0] = side::RIGHT;
        break;

    default:
        branchPositions[0] = side::NONE;
        break;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
