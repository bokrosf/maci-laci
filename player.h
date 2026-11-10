#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTimer>
#include <QSoundEffect>
#include "movingcharacters.h"

class Player : public MovingCharacters
{
    Q_OBJECT
private:
    int slowMoDuration;  //ennyi ideig lehet használni, másodpercben értve
    int slowDownAmount;  //ennyivel lassítja le a vadőröket, ezredmásodpercben értve
    QTimer *slowMotion;
    QSoundEffect *slowMoSound;

public:
    Player(int x, int y);
    ~Player();

    int  getSlowMoDuration() const;
    bool isSlowMoActive() const;
    void slowMotionUsed();
    void deactivateSlowMotion();
    void addTime(int msec);
    void setSlowDownAmount(int msec);

signals:
    void slowMotionActivated(int time);
    void slowMotionDeactivated();
    void slowMoDurationChanged();

private slots:
    void slowDownTime();


};

#endif // PLAYER_H
