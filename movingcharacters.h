#ifndef MOVINGCHARACTERS_H
#define MOVINGCHARACTERS_H

#include<QObject>

class MovingCharacters : public QObject
{
    Q_OBJECT
protected:
    int x;
    int y;

public:
    MovingCharacters(int x, int y);
    virtual ~MovingCharacters();

    int posX() const;
    int posY() const;

    void setPosX(int x);
    void setPosY(int y);
};

#endif // MOVINGCHARACTERS_H
