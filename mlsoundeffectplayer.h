#ifndef MLSOUNDEFFECTPLAYER_H
#define MLSOUNDEFFECTPLAYER_H

#include <QObject>
#include <QSoundEffect>
#include <QString>
#include "gamecontroller.h"

#define SOUND_PATH "sounds/"

class MLSoundEffectPlayer : public QObject
{
    Q_OBJECT
public:
    MLSoundEffectPlayer(QObject *parent = 0);
    ~MLSoundEffectPlayer();

private:
    QSoundEffect *grassSound;

public slots:

};

#endif // SOUNDEFFECTPLAYER_H
