#ifndef MACILACIWIDGET_H
#define MACILACIWIDGET_H

#include <QWidget>
#include <QVBoxLayout> //ebben van a HBox- és alatta a GRid layout
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QVector>
#include <QKeyEvent>
#include <QTime>
#include <QProgressBar>
#include <QPixmap>
#include <QMap>
#include <QComboBox>
#include "gamecontroller.h"

class MaciLaciWidget : public QWidget
{
    Q_OBJECT

public:
    MaciLaciWidget(QWidget *parent = 0);
    ~MaciLaciWidget();

private:
    GameController *gameController;

    QVector<QVector<QLabel*> > gameField;

    QVBoxLayout *mainLayout;
    QHBoxLayout *interactionPanel;
    QGridLayout *gameFieldLayout;
    QVBoxLayout *gameStatsLayout;
    QVBoxLayout *slowMoBarLayout;
    QComboBox *levelSelector;

    QPushButton *newGameButton;

    QLabel *picknickBaskets;
    QLabel *gameTime;
    QLabel *slowMoLabel;

    QProgressBar *slowMoBar;

    void keyPressEvent(QKeyEvent *event);
    void resetGameField();

    void initMainLayout();
    void initLevelSelectorLayout();
    void initNewGameButton();
    void initSlowmoBar();
    void initGameStatsPanel();

private slots:
    void newGameStarted();
    void refreshTime(QTime elapsedTime);
    void refreshPicknickBasketNumber(int currBasketNumber, int allBasketNumber);
    void refreshSlowMoBar(int remainingTime);
    void initialzeMap(int gameFieldHeight, int gameFieldWidth);
    void gameFieldImageChanged(int x, int y, Platforms changeTo);
    void gameEnded(GameEnding endedWith);
};

#endif // MACILACIWIDGET_H
