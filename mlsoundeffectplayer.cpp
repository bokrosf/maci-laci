#include <QString>
#include "mlsoundeffectplayer.h"

//public:
MLSoundEffectPlayer::MLSoundEffectPlayer(QObject *parent) : QObject(parent)
{
    grassSound = new QSound("sounds/grass.wav");
}

MLSoundEffectPlayer::~MLSoundEffectPlayer()
{

}

//public slots:

