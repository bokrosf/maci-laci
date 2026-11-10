#ifndef RANGER_H
#define RANGER_H

#include "movingcharacters.h"

class Ranger : public MovingCharacters
{
    Q_OBJECT
private:
    int horizontal;
    int vertical;
    bool standingOnPicknickBasket;

public:
    Ranger(int x, int y, int horizontal, int vertical);
    ~Ranger();

    int dirHorizontal() const;
    int dirVertical() const;
    bool isStandingOnPicknickBasket() const;
    int picknickPosX() const;
    int picknickPosY() const;

    void setDirection(int horizontal, int vertical);
    void turnBack();
    void standOnPicknickBasket();
    void leavePicknickBasket();
};

#endif // RANGER_H
