#include <QUrl>
#include "player.h"
#include "soundsources.h"

Player::Player(int x, int y):
    MovingCharacters(x, y), slowMoDuration(0)
{
    slowMotion = new QTimer;
    slowMoSound = new QSoundEffect();
    slowMoSound->setSource(QUrl::fromLocalFile(DURING_SLOWMO_SOUND));
    slowMoSound->setLoopCount(QSoundEffect::Loop::Infinite);
    connect(slowMotion, SIGNAL(timeout()), this, SLOT(slowDownTime()) );
}

Player::~Player()
{
    slowMotion->stop();
    disconnect(slowMotion, SIGNAL(timeout()), this, SLOT(slowDownTime()) );
    delete slowMotion;
    slowMotion = nullptr;
    delete slowMoSound;
    slowMoSound = nullptr;
}

int Player::getSlowMoDuration() const
{
    return slowMoDuration;
}

bool Player::isSlowMoActive() const
{
    return slowMotion->isActive();
}

void Player::slowMotionUsed()
{
    if( !slowMotion->isActive() && slowMoDuration != 0 )
    {
        QSoundEffect sound;
        sound.setSource(QUrl::fromLocalFile(ENTER_SLOWMO_SOUND));
        sound.play();
        slowMoSound->play();
        slowMotion->start(1);
        slowMotionActivated(slowDownAmount);
    }
    else if( slowMotion->isActive() )
    {
        deactivateSlowMotion();
    }
}

void Player::deactivateSlowMotion()
{
    QSoundEffect sound;
    sound.setSource(QUrl::fromLocalFile(EXIT_SLOWMO_SOUND));
    sound.play();
    slowMoSound->stop();
    slowMotion->stop();
    slowMotionDeactivated();
}

void Player::addTime(int msec)
{
    if (msec >= 0)
    {
        slowMoDuration += msec;
        slowMoDurationChanged();
    }
}

void Player::setSlowDownAmount(int msec)
{
    slowDownAmount = msec;
}

//Private Slot:
void Player::slowDownTime()
{
    if( slowMoDuration > 0 )
    {
        --slowMoDuration;
        slowMoDurationChanged();
    }
    else
    {
        deactivateSlowMotion();
        slowMotionDeactivated();
    }
}
