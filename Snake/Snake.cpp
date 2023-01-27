/*
 * Coded by Davi A.S.R.O
 * You are free to use it or copy it
 * just mention that I wrote the code
*/

// Cool libraries
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

// Snake classes
#include "resources/snake.hpp"

// Boring stuff
using namespace std;
using namespace sf;

int
main(void)
{
    // Window management
    RenderWindow window(VideoMode(DISPLAY, DISPLAY), "Snake by Davi A.S.R.O", Style::Titlebar | Style::Close);
    window.setFramerateLimit(120);
    // ----------

    // Prepare death messages
    Texture loser;
    Texture jinx;
    jinx.loadFromFile("resources/menu.png");

    //Music please
    Music jazz;
    if(!jazz.openFromFile("resources/techno-1.ogg"))
    {
        return 1;
    }

    Music elevator;
    if(!elevator.openFromFile("resources/local-forecast.ogg"))
    {
        return 2;
    }
    bool gameStarted = false;

    Sprite menu;
    menu.setTexture(jinx);
    menu.setTextureRect(IntRect(0, 0, DISPLAY, DISPLAY));

    while(window.isOpen())
    {
        // Basic things windows do
        Event event;
        if(window.pollEvent(event))
        {
            if(event.type == Event::Closed)
            {
                jazz.stop();
                window.close();
            }
        }

        // Menu
        if(elevator.getStatus() == Music::Stopped)
        {
            elevator.play();
        }

        window.clear();
        window.draw(menu);
        window.display();

        if(Keyboard::isKeyPressed(Keyboard::Space))
        {
            gameStarted = true;
            elevator.stop();
        }
        else if(Keyboard::isKeyPressed(Keyboard::H))
        {
            window.clear();
            menu.setTextureRect(IntRect(600, 0, DISPLAY, DISPLAY));
            window.draw(menu);
            window.display();
            sleep(seconds(5));
            menu.setTextureRect(IntRect(0, 0, DISPLAY, DISPLAY));
        }

        // Game time!
        if(gameStarted)
        {
            // Set the snake and the game
            Snake player;
            player.Set();
    
            // Fruit time
            Fruit apple;
            apple.Set();
            apple.Show();

            // Body linker (Imagine parts of body floating around ;))
            Body *headSnake = NULL;
            
            do
            {
                Event event;
                if(window.pollEvent(event))
                {
                    if(event.type == Event::Closed)
                    {
                        window.close();
                    }
                }
                // Music stoped? Play it again!
                if(jazz.getStatus() == Music::Stopped)
                    jazz.play();

                // Moves the snake
                // Up
                if(Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
                {
                    player.Move(2);
                    if(player.Size > 1)
                        headSnake->Follow(player.previousX, player.previousY);
                }
                // Down
                else if(Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
                {
                    player.Move(3);
                    if(player.Size > 1)
                        headSnake->Follow(player.previousX, player.previousY);
                }
                // Left
                else if(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
                {
                    player.Move(1);
                    if(player.Size > 1)
                        headSnake->Follow(player.previousX, player.previousY);
                }
                // Right
                else if(Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
                {
                    player.Move(0);
                    if(player.Size > 1)
                        headSnake->Follow(player.previousX, player.previousY);
                }
                // Same direction as before
                else
                {
                    player.Move(player.direction);
                    if(player.Size > 1)
                        headSnake->Follow(player.previousX, player.previousY);
                }
                // -------------------------------------------------------------

                // Check if the snake hit itself
                for(Body *tmp = headSnake; tmp != NULL; tmp = tmp->follower)
                {
                    if(tmp->Hit(player.GetX(), player.GetY()))
                    {
                        player.Dead = !player.Dead;
                        break;
                    }
                }

                // Grow the snake
                if(apple.Eaten(player.GetX(), player.GetY()))
                {
                    Body *buffer = new Body;
                    // A little fat
                    if(player.Size > 1)
                    {
                        headSnake->New_body(buffer);
                        player.Size++;
                    }
                    // Too skinny
                    else
                    {
                        headSnake = buffer;
                        player.Size++;
                    }

                    apple.Show();
                }

                // Draw everything that happened
                window.clear();
                for(Body *tmp = headSnake; tmp != NULL; tmp = tmp->follower)
                {
                    window.draw(tmp->Part);
                }
                window.draw(apple.f);
                window.draw(player.Head);
                window.display();

                sf::sleep(milliseconds(100));
            }while(!player.Dead && !player.Win());

            sf::sleep(milliseconds(100));

            // Cool death messages
            char name[40];
            std::sprintf(name, "resources/death_messages/message%d.png", rand() % 4);

            // Loser
            loser.loadFromFile(name);
            Sprite lostImage;
            lostImage.setTexture(loser);

            // Show your last words
            window.clear();
            window.draw(lostImage);
            window.display();

            // Im sleepy right now
            sf::sleep(seconds(7));

            // Explosions (no memory leaks allowed!)
            headSnake->Kill();
            delete headSnake;
            gameStarted = false;
            jazz.stop();
        }

    }

}