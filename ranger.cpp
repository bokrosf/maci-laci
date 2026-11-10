#include "ranger.h"

Ranger::Ranger(int x, int y, int horizontal, int vertical) : MovingCharacters(x, y)
{
    setDirection(horizontal, vertical);
    standingOnPicknickBasket  = false;
}

Ranger::~Ranger()
{

}

int Ranger::dirHorizontal() const
{
    return horizontal;
}

int Ranger::dirVertical() const
{
    return vertical;
}

bool Ranger::isStandingOnPicknickBasket() const
{
    return standingOnPicknickBasket;
}

void Ranger::setDirection(int horizontal, int vertical)
{
    Ranger::horizontal = horizontal;
    Ranger::vertical   = vertical;
}

void Ranger::turnBack()
{
    setDirection(horizontal * (-1), vertical * (-1) );  //kizáró vagyos meggondolási alapon, csak az egyik lehet 0
}

void Ranger::standOnPicknickBasket()
{
    standingOnPicknickBasket = true;
}

void Ranger::leavePicknickBasket()
{
    standingOnPicknickBasket = false;
}
