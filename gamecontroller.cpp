#include <QFile>
#include <QIODevice>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include "gamecontroller.h"
#include "soundsources.h"

//Private függvények:
QTime GameController::getElapsedTime()
{
    QTime result(elapsedTime->hour(),
                 elapsedTime->minute(),
                 elapsedTime->second(),
                 elapsedTime->msec() );

    return result;
}

bool GameController::isValidArea(int x, int y)
{
    return (0 <= x) && (x < mapHeight) && (0 <= y) && (y < mapWidth);
}

bool GameController::isSteppableMaze(int x, int y)
{
    if( isValidArea(x, y) )
    {
        switch( Forest[x][y] )
        {
            case PICKNICKBASKET: return true; break;
            case GRASS:          return true; break;
            default: return false;
        }
    }
    else
    {
        return false;
    }
}

void GameController::movePlayer(int dirX, int dirY)
{
    if( isSteppableMaze(player->posX() + dirX, player->posY() + dirY) )
    {
        Forest[ player->posX() ][ player->posY() ] = GRASS; //ahol áll a játékos azt lecserélem fűre
        platformChanged(player->posX(), player->posY(), GRASS); //értesítem a nézetet, hogy változtassa meg fűre a játékos képét

        player->setPosX(player->posX() + dirX);
        player->setPosY(player->posY() + dirY);


        if( Forest[ player->posX() ][ player->posY() ] == PICKNICKBASKET ) //megnézem, hogy piknikkosárra lép-e a játékos
        {
            QSoundEffect sound;
            sound.setSource(QUrl::fromLocalFile(PICKNICK_SOUND));
            sound.play();
            Forest[ player->posX() ][ player->posY() ] = PLAYER;
            platformChanged(player->posX(), player->posY(), PLAYER);


            for(int i = 0; i < Rangers.size(); ++i) //meglesem, hogy tiltott helyere lépett-e
            {
                checkSuspiciousActivity(Rangers[i]);
            }

            pickUpPicknickBasket();
        }
        else  //ha nem piknikkosárra léptem, akkor fűre léptem
        {
            playGrassSound();
            Forest[ player->posX() ][ player->posY() ] = PLAYER;
            platformChanged(player->posX(), player->posY(), PLAYER);

            for(int i = 0; i < Rangers.size(); ++i) //meglesem, hogy tiltott helyere lépett-e
            {
                checkSuspiciousActivity(Rangers[i]);
            }
        }
    }
    else if (isValidArea(player->posX() + dirX, player->posY() + dirY)) //olyan mezők, amelyekre nem lehet rálépni
    {
        QSoundEffect sound;

        switch(Forest[player->posX() + dirX][player->posY() +dirY])
        {
        case TREE:
            sound.setSource(QUrl::fromLocalFile(TREE_SOUND));
            sound.play();
            break;
        case ROCK:
            sound.setSource(QUrl::fromLocalFile(ROCK_SOUND));
            sound.play();
            break;
        default:
            break;
        }
    }


}

void GameController::moveRanger(Ranger *ranger)
{
    checkSuspiciousActivity(ranger);
    if( !gameEnded )
    {
        //Ha már piknikkosáron áll, akkor előbb vissza kell tennie
        if( ranger->isStandingOnPicknickBasket() )
        {
            Forest[ ranger->posX() ][ ranger->posY() ] = PICKNICKBASKET;
            platformChanged(ranger->posX(), ranger->posY(), PICKNICKBASKET);
            ranger->leavePicknickBasket();
        }
        else
        {
            Forest[ ranger->posX() ][ ranger->posY() ] = GRASS;
            platformChanged(ranger->posX(), ranger->posY(), GRASS);
        }

        ranger->setPosX( ranger->posX() + ranger->dirHorizontal() );
        ranger->setPosY( ranger->posY() + ranger->dirVertical()   );

        //Ha piknikkosárra lépne, akkor előbb fel kell vennie
        if( Forest[ ranger->posX() ][ ranger->posY() ] == PICKNICKBASKET )
        {
            ranger->standOnPicknickBasket();
        }

        Forest[ ranger->posX() ][ ranger->posY() ] = RANGER;
        platformChanged(ranger->posX(), ranger->posY(), RANGER);
    }
}

void GameController::checkSuspiciousActivity(Ranger *ranger)
{
    if( !gameEnded && abs(ranger->posX() - player->posX()) <= 1 && abs(ranger->posY() - player->posY()) <= 1 )
    {
        gameEnded = true;
        pauseGame();
        //QSound::play(ARREST_SOUND); //vereség hang
        gameOver(ARREST);
    }
}

void GameController::pickUpPicknickBasket()
{
    ++pickedUpPicknickBasketNumber;
    picknickBasketPickedUp(pickedUpPicknickBasketNumber, picknickBasketNumber); //meghívja a nézet, kosár frissítő függvényét
    player->addTime(1000);

    if( !gameEnded && pickedUpPicknickBasketNumber == picknickBasketNumber )
    {
        gameEnded = true;
        pauseGame();
        //QSound::play(VICTORY_SOUND); //győzelmi hang
        gameOver(VICTORY);
    }
}

void GameController::readGameData(int levelID)
{
    QFile levelFile(":/levels/level" + QString::number(levelID) + ".lvl");
    levelFile.open(QIODevice::ReadOnly);
    QTextStream fileStream(&levelFile);

    fileStream >> mapHeight;
    fileStream >> mapWidth;
    Forest.resize(mapHeight);

    for(int i = 0; i < Forest.size(); ++i)
    {
        Forest[i].resize(mapWidth);
        for(int j = 0; j < Forest[i].size(); ++j)
        {
            Forest[i][j] = GRASS;
        }
    }

    readPlatforms(fileStream);
    levelFile.close();
}

void GameController::readPlatforms(QTextStream &fileStream)
{
    while( !fileStream.atEnd() )
    {
        int currentPlatformNumber;
        QString currentPlatformType;
        fileStream >> currentPlatformNumber;
        fileStream >> currentPlatformType;

        //egy jatekmező típus elemeinek beolvasása és elhelyezése
        for(int i = 0; i < currentPlatformNumber; ++i)
        {
            int posX, posY;
            fileStream >> posX >> posY;

            if( currentPlatformType == "picknick" )
            {
                Forest[posX][posY] = PICKNICKBASKET;
                ++picknickBasketNumber;
            }
            else if( currentPlatformType == "tree" )
            {
                Forest[posX][posY] = TREE;
            }
            else if( currentPlatformType == "rock" )
            {
                Forest[posX][posY] = ROCK;
            }
            else if( currentPlatformType == "ranger" )
            {
                int horizontal, vertical;
                fileStream >> horizontal >> vertical;
                Forest[posX][posY] = RANGER;
                Rangers.append(new Ranger(posX, posY, horizontal, vertical) );
            }
        }
    }
}

void GameController::playGrassSound()
{
    static const QString grass_sounds[] = { GRASS_SOUND_1 , GRASS_SOUND_2 };
    static char index = 0;

    QSoundEffect sound;
    sound.setSource(QUrl::fromLocalFile(grass_sounds[(index & 1)]));
    sound.play();
    ++index;
}

//Public függvények:
GameController::GameController()
{
    player      = 0;
    elapsedTime = 0;
    gameClock   = 0;
    patrolling  = 0;
}

GameController::~GameController()
{
    resetGame();
}

int GameController::getPicknickBasketNumber() const
{
    return picknickBasketNumber;
}

int GameController::getMaxSlowMoDuration() const
{
    return maxSlowMoDuration;
}

void GameController::newGame(int levelID)
{
    resetGame();
    readGameData(levelID);

    player       = new Player(0, 0);
    elapsedTime  = new QTime(0, 0, 0, 0);
    patrolling   = new QTimer;
    gameClock    = new QTimer;

    maxSlowMoDuration = 1000 * picknickBasketNumber;
    rangerSpeed  = 500;
    player->setSlowDownAmount(rangerSpeed);
    Forest[0][0] = PLAYER;
    gameEnded    = false;

    connect(gameClock,  SIGNAL(timeout()), this, SLOT(incrementElapsedTime()) );
    connect(patrolling, SIGNAL(timeout()), this, SLOT(rangersPatrol()) );
    connect(player, SIGNAL(slowMotionActivated(int)), this, SLOT(slowMotionActivated(int))  );
    connect(player, SIGNAL(slowMotionDeactivated()),  this, SLOT(slowMotionDeactivated())   );
    connect(player, SIGNAL(slowMoDurationChanged()),  this, SLOT(sendRemainingSlowMoTime()) );

    mapLoaded(mapHeight, mapWidth); //A view elő tudja készíteni a megjelenítéshez az adatszerkezetét
    picknickBasketPickedUp(pickedUpPicknickBasketNumber, picknickBasketNumber); //Értesíti a view-t a kosarak számáról
    timeElapsed(getElapsedTime()); // nullázza a nézeten az időt
    sendRemainingSlowMoTime();

    //A view-t értesíti a mezők megváltozásáról
    for(int i = 0; i < Forest.size(); ++i)
    {
        for(int j = 0; j < Forest[i].size(); ++j)
        {
            platformChanged(i, j, Forest[i][j]);
        }
    }

    continueGame(); // ez indítja el a játékot
}

void GameController::pauseGame()
{
    gamePaused = true;
    gameClock->stop();
    patrolling->stop();

    if( player->isSlowMoActive() )
    {
        player->deactivateSlowMotion();
    }

}

void GameController::continueGame()
{
    gamePaused = false;
    gameClock->start(1000);
    patrolling->start(rangerSpeed);
}

void GameController::resetGame()
{
    mapHeight = 0;
    mapWidth  = 0;
    picknickBasketNumber = 0;
    pickedUpPicknickBasketNumber = 0;

    Forest.clear();
    delete player;

    for (int i = 0; i < Rangers.size(); ++i)
    {
        delete Rangers[i];
    }

    Rangers.clear();
    delete elapsedTime;
    delete gameClock;
    delete patrolling;

    player      = 0;
    elapsedTime = 0;
    gameClock   = 0;
    patrolling  = 0;
}

void GameController::keyPressed(int pressedKey)
{
    //csak akkor mozoghat a játékos, ha nincs vége a játéknak, vagy nincs megállítva
    if( !gamePaused && !gameEnded)
    {
        switch( pressedKey )
        {
            case Qt::Key_Up     : movePlayer(-1, 0); break;
            case Qt::Key_Down   : movePlayer(1, 0);  break;
            case Qt::Key_Left   : movePlayer(0, -1); break;
            case Qt::Key_Right  : movePlayer(0,  1); break;
            case Qt::Key_Escape : pauseGame();       break;
            case Qt::Key_Shift  : player->slowMotionUsed(); break;
        }
    }
    else if( !gameEnded && pressedKey == Qt::Key_Escape )
    {
        continueGame();
    }
}

//Private slotok:
void GameController::incrementElapsedTime()
{
    *elapsedTime = elapsedTime->addSecs(1); //az addSecs(int) függvény egy QTime objektumot ad vissza eredményül
    timeElapsed(getElapsedTime());
}

//Ha a gamePaused értéke igaz, akkor nem hívódik meg
void GameController::rangersPatrol()
{
    for(int i = 0; i < Rangers.size(); ++i)
    {
        if( !gameEnded )
        {
            if( isSteppableMaze(Rangers[i]->posX() + Rangers[i]->dirHorizontal(), Rangers[i]->posY() + Rangers[i]->dirVertical()) )
            {
                moveRanger(Rangers[i]);
            }
            else
            {
                //Ha nem tud rálépni a mezőre, akkor meg kell fordulnia, utána már tovább tud haladni
                Rangers[i]->turnBack();
                moveRanger(Rangers[i]);
            }
            checkSuspiciousActivity(Rangers[i]);
        }
    }
}

void GameController::slowMotionActivated(int time)
{
    patrolling->setInterval(rangerSpeed + time);
}

void GameController::slowMotionDeactivated()
{
    patrolling->setInterval(rangerSpeed);
}

void GameController::sendRemainingSlowMoTime()
{
    slowMoDurationChanged(player->getSlowMoDuration());
}
