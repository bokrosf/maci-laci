#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QVector>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <QTextStream>
#include <QString>
#include "player.h"
#include "ranger.h"
#include "mlsoundeffectplayer.h"

enum Platforms  { PLAYER, RANGER, PICKNICKBASKET, GRASS, TREE, ROCK };
enum GameEnding { VICTORY, ARREST };

class GameController : public QObject
{
    Q_OBJECT
private:
    int mapWidth;
    int mapHeight;
    int picknickBasketNumber;
    int pickedUpPicknickBasketNumber;
    int maxSlowMoDuration;
    int rangerSpeed;
    bool gamePaused;
    bool gameEnded;

    QVector<QVector<Platforms> > Forest;
    Player *player;
    QVector<Ranger*> Rangers;
    QTime  *elapsedTime;
    QTimer *patrolling;
    QTimer *gameClock;

    QTime getElapsedTime();
    bool isValidArea(int x, int y);
    bool isSteppableMaze(int x, int y);
    void movePlayer(int dirX, int dirY);
    void moveRanger(Ranger *ranger);
    void checkSuspiciousActivity(Ranger *ranger);
    void pickUpPicknickBasket();
    void readGameData(const QString &levelName);
    void readPlatforms(QTextStream &fileStream);
    void resetGame();
    void playGrassSound();

public:
    GameController();
    ~GameController();

    int getPicknickBasketNumber() const;
    int getMaxSlowMoDuration() const;

    void newGame(const QString &levelName);
    void pauseGame();
    void continueGame();
    void keyPressed(int pressedKey);

signals:
    void picknickBasketPickedUp(int pickedUpNum, int allBasketNum); //felvett kosarak számát adja tovább
    void gameOver(GameEnding ending);             // tovább adja, hogy nyert-e vagy vesztett a játékos
    void timeElapsed(const QTime &currentTime);   // a játékban eltelt időt adja tovább
    void mapLoaded(int height, int width);
    void platformChanged(int x, int y, Platforms platformType); //melyik pozíción változott meg a játékmező és mire
    void slowMoDurationChanged(int remainingTime);
    void platformSoundRequest(Platforms platform);
    void endingSoundRequest(GameEnding ending);

private slots:
    void incrementElapsedTime();
    void rangersPatrol();
    void slowMotionActivated(int time);
    void slowMotionDeactivated();
    void sendRemainingSlowMoTime();
};

#endif // GAMECONTROLLER_H
