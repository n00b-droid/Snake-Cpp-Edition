/*
 * Licensa de uso, só para o caso de voce tentar dizer que esse codigo que eu escrevi com muito (muitissimo)
 * esforço não seja roubado por um zé mané sem nada o que fazer!
 * 
 * English:
 * Copyright to not let you say that this code I coded with a lot of effort was written by a script kiddie
 * with not to do.
 */

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

#define DISPLAY 600

using namespace sf;

// Standard size of the images
const int textSize = 24;

// An GPS for the snake don't get lost
bool snakeThere[DISPLAY / textSize][DISPLAY / textSize];

class Body
{
    private:
        // Current position of tis body part
        int x = 0;
        int y = 0;

        // Past position to be followed by other body part
        int previousX;
        int previousY;

        // Texture for the Sprite
        Texture jinx;

    public:

        // Who is following me?
        Body *follower = NULL;
        Sprite Part;

        // Time to get fat
        void New_body(Body *tail)
        {
            // Using recursion to find the tail
            if(follower == NULL)
            {
                follower = tail;
            }
            else
            {
                follower->New_body(tail);
            }

            // Loading the sprite
            jinx.loadFromFile("resources/Snake.png");
            Part.setTexture(jinx);
            Part.setTextureRect(IntRect(24 * 2, 0, textSize, textSize));
        }

        // Not you bro, the snake
        void Kill()
        {
            Body *tmp;
            while(follower != NULL)
            {
                tmp = follower;
                follower = follower->follower;
                delete(tmp);
            }
        }

        // Follow the head or body part in the front
        void Follow(int pX, int pY)
        {
            /*
             * pX = previous X
             * pY = previous Y 
            */
            Part.setPosition(pX * textSize, pY * textSize);
            previousX = x;
            x = pX;
            previousY = y;
            y = pY;

            // Where was I? 
            snakeThere[x][y] = true;
            snakeThere[previousX][previousY] = false;
            if(follower != NULL)
            {
                follower->Follow(this->previousX, this->previousY);
            }
        }

        // Why are you slapping yourself
        bool Hit(int headPosx, int headPosy)
        {
            if(headPosx == x && headPosy == y)
            {
                return true;
            }
            return false;
        }

        // Failure OOP (Object Oriented Programing)
        int Y()
        {
            return this->y;
        }
        int X()
        {
            return this->x;
        }
};


class Fruit
{
    private:
        // Position of the fruit
        int x;
        int y;

        Texture jinx;
        
    public:
        // Oh a foto of myself
        Sprite f;

        // Load standard values and images
        void Set(void)
        {
            jinx.loadFromFile("resources/Snake.png");
            srand(time(NULL));
            x = y = 0;
            f.setTexture(jinx);
            f.setTextureRect(IntRect(0, 0, textSize, textSize));
        }

        // Set the position of the fruit
        void Show()
        {
            // Is this gambling?
            x = rand() % (DISPLAY / textSize);
            y = rand() % (DISPLAY / textSize);

            // Not spawnning the fruit inside the snake
            while(snakeThere[x][y] == true)
            {
                x = rand() % (DISPLAY / textSize);
                y = rand() % (DISPLAY / textSize);
            }

            f.setPosition(x * textSize, y * textSize);
        }

        // Check if its eaten
        bool Eaten(int headPosx, int headPosy)
        {
            if(headPosx == x && headPosy == y)
            {
                return true;
            }
            return false;
        }

};


class Snake
{
    private:
        // Position of the head
        int x;
        int y;
        
        Texture jinx;
        
    
    public:
        // Previous position of the head
        int previousX;
        int previousY;

        // Dead or not? This is the question
        bool Dead = false;
        Sprite Head;

        // Almost too skinny
        int Size = 1;

        // Sets standard sprites and values
        void Set()
        {
            x = 0;
            y = 0;
            direction = 0;
            jinx.loadFromFile("resources/Snake.png");
            Head.setTexture(jinx);
            Head.setTextureRect(IntRect(24, 0, textSize, textSize));
            
        }

        bool Win()
        {
            for(int i = 0; i < DISPLAY / textSize; i++)
            {
                for(int j = 0; j < DISPLAY / textSize; j++)
                {
                    if(!snakeThere[i][j])
                    {
                        return false;
                    }
                }
                return true;
            }
        }

        // 0 = right, 1= left, 2 = up and 3 = down (Looks like FNF)
        int direction;

        // Move it (d = new direction)
        void Move(int d)
        {
            // Check if the snake hitted an wall because the player is a failure
            Vector2f nextBox;
            
            if(Dead)
            {
                return;
            }

            // Don't let the snake go on the opposite direction
            if((d == 0 && direction == 1) ||
               (d == 1 && direction == 0) ||
               (d == 2 && direction == 3) ||
               (d == 3 && direction == 2))
            {
                d = direction;
            }

            /*
             * Moves the snake and tell where
             * it was, to be followed
             */
            switch (d)
            {
                // Turn right
                case 0:
                    nextBox.x = this->x + 1;
                    if(nextBox.x + 1 > DISPLAY / textSize)
                    {
                        Dead = !Dead;
                        return;
                    }
                    nextBox.x++;
                    previousX = x;
                    previousY = y;
                    x++;
                    snakeThere[x][y] = true;
                    snakeThere[previousX][previousY] = false;
                    Head.setPosition(x * textSize, y * textSize);
                    direction = 0;
                    break;

                // Turn left
                case 1:
                    nextBox.x = this->x - 1;
                    if(nextBox.x < 0)
                    {
                        Dead = !Dead;
                        return;
                    }
                    nextBox.x--;
                    previousX = x;
                    previousY = y;
                    x--;
                    snakeThere[x][y] = true;
                    snakeThere[previousX][previousY] = false;
                    Head.setPosition(x * textSize, y * textSize);
                    direction = 1;
                    break;

                // Go up
                case 2:
                    nextBox.y = this->y - 1;
                    if(nextBox.y < 0)
                    {
                        Dead = !Dead;
                        return;
                    }
                    nextBox.y--;
                    previousY = y;
                    previousX = x;
                    y--;
                    snakeThere[x][y] = true;
                    snakeThere[previousX][previousY] = false;
                    Head.setPosition(x * textSize, y * textSize);
                    direction = 2;
                    break;

                // Go down
                case 3:
                    nextBox.y = this->y + 1;
                    if(nextBox.y + 1 > DISPLAY / textSize)
                    {
                        Dead = !Dead;
                        return;
                    }
                    nextBox.x++;
                    previousY = y;
                    previousX = x;
                    y++;
                    snakeThere[x][y] = true;
                    snakeThere[previousX][previousY] = false;
                    Head.setPosition(x * textSize, y * textSize);
                    direction = 3;
                    break;
            }
        }

        // Return the x and y position respectively
        // Again boring OOP
        int GetX()
        {
            return this->x;
        }
        int GetY()
        {
            return this->y;
        }
};