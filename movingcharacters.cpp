#include "movingcharacters.h"

MovingCharacters::MovingCharacters(int x, int y)
{
    MovingCharacters::x = x;
    MovingCharacters::y = y;
}

MovingCharacters::~MovingCharacters()
{

}

int MovingCharacters::posX() const
{
    return x;
}

int MovingCharacters::posY() const
{
    return y;
}

void MovingCharacters::setPosX(int x)
{
    MovingCharacters::x = x;
}

void MovingCharacters::setPosY(int y)
{
    MovingCharacters::y = y;
}
