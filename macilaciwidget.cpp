#include <QPixmap>
#include <QMessageBox>
#include "macilaciwidget.h"
#include "gameoverwidget.h"
#include "soundsources.h"

//private függvények:
void MaciLaciWidget::keyPressEvent(QKeyEvent *event)
{
    gameController->keyPressed(event->key());
}

void MaciLaciWidget::resetGameField()
{
    for(int i = 0; i < gameField.size(); ++i)
    {
        for(int j = 0; j < gameField[i].size(); ++j)
        {

            gameFieldLayout->removeWidget(gameField[i][j]);
            delete gameField[i][j];
        }
    }
    gameField.clear();
}

int MaciLaciWidget::levelSelected()
{
    if( level1->isChecked() )
    {
        return 1;
    }
    else if( level2->isChecked() )
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

void MaciLaciWidget::initMainLayout()
{
    mainLayout = new QVBoxLayout(this);
    interactionPanel = new QHBoxLayout;
    gameFieldLayout = new QGridLayout;
    gameFieldLayout->setSpacing(0);
    mainLayout->addLayout(interactionPanel);
    mainLayout->addLayout(gameFieldLayout);
}

void MaciLaciWidget::initLevelSelectorLayout()
{
    levelSelectorLayout = new QHBoxLayout;
    levelSelector = new QGroupBox("Pályák listája:");
    level1 = new QRadioButton("Pálya 1");
    level1->setChecked(true);
    level2 = new QRadioButton("Pálya 2");
    level3 = new QRadioButton("Pálya 3");

    levelSelectorLayout->addWidget(level1);
    levelSelectorLayout->addWidget(level2);
    levelSelectorLayout->addWidget(level3);
    levelSelectorLayout->setSizeConstraint(QLayout::SetFixedSize);

    levelSelector->setLayout(levelSelectorLayout);
    interactionPanel->addWidget(levelSelector);
}

void MaciLaciWidget::initNewGameButton()
{
    newGameButton = new QPushButton("Új játék");
    newGameButton->setFixedWidth(120);
    interactionPanel->addWidget(newGameButton);
}

void MaciLaciWidget::initSlowmoBar()
{
    slowMoLabel = new QLabel("időlassítás");
    slowMoBar   = new QProgressBar;
    slowMoBar->setFixedSize(150,20);
    slowMoBarLayout = new QVBoxLayout;
    slowMoBarLayout->addWidget(slowMoLabel);
    slowMoBarLayout->addWidget(slowMoBar);
    interactionPanel->addLayout(slowMoBarLayout);
}

void MaciLaciWidget::initGameStatsPanel()
{
    gameStatsLayout = new QVBoxLayout;
    picknickBaskets = new QLabel("kosarak: ");
    gameTime = new QLabel("eltelt idő: ");
    gameStatsLayout->addWidget(picknickBaskets);
    gameStatsLayout->addWidget(gameTime);
    interactionPanel->addLayout(gameStatsLayout);
}

//public függvények
MaciLaciWidget::MaciLaciWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Maci Laci 1.0");
    gameController = new GameController;

    initMainLayout();
    initLevelSelectorLayout();
    initNewGameButton();
    initSlowmoBar();
    initGameStatsPanel();

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    connect(newGameButton,  SIGNAL(clicked(bool)), this, SLOT(newGameStarted()) );
    connect(gameController, SIGNAL(mapLoaded(int,int)), this, SLOT(initialzeMap(int,int)) );
    connect(gameController, SIGNAL(timeElapsed(QTime)), this, SLOT(refreshTime(QTime)) );
    connect(gameController, SIGNAL(picknickBasketPickedUp(int,int)), this, SLOT(refreshPicknickBasketNumber(int,int)) );
    connect(gameController, SIGNAL(slowMoDurationChanged(int)), this, SLOT(refreshSlowMoBar(int)) );
    connect(gameController, SIGNAL(platformChanged(int,int,Platforms)), this, SLOT(gameFieldImageChanged(int,int,Platforms)) );
    connect(gameController, SIGNAL(gameOver(GameEnding)), this, SLOT(gameEnded(GameEnding)) );
}

MaciLaciWidget::~MaciLaciWidget()
{

}

//private SLOT-ok:
void MaciLaciWidget::newGameStarted()
{
    //hang lejátszása
    gameController->newGame(levelSelected());
    slowMoBar->setMaximum(gameController->getMaxSlowMoDuration());
    setFocus();
}

void MaciLaciWidget::refreshTime(QTime elapsedTime)
{
    gameTime->setText("eltelt idő: " + elapsedTime.toString("mm:ss") );
}

void MaciLaciWidget::refreshPicknickBasketNumber(int currBasketNumber, int allBasketNumber)
{
    picknickBaskets->setText("kosarak: " + QString::number(currBasketNumber) + " / " + QString::number(allBasketNumber));
}

void MaciLaciWidget::refreshSlowMoBar(int remainingTime)
{
    slowMoBar->setValue(remainingTime);
}

void MaciLaciWidget::gameEnded(GameEnding endedWith)
{
    switch( endedWith )
    {
        GameOverWidget *gameOverWidget;
        case VICTORY:
        {
            gameOverWidget = new GameOverWidget("Nyertél", VICTORY_SOUND);
            gameOverWidget->show();
            break;
        }
        case ARREST:
        {
            gameOverWidget = new GameOverWidget("Vesztettél", ARREST_SOUND);
            gameOverWidget->show();
            break;
        }
        delete gameOverWidget;
        gameOverWidget = nullptr;
    }
    newGameButton->setFocus();
}

void MaciLaciWidget::initialzeMap(int gameFieldHeight, int gameFieldWidth)
{
    resetGameField();

    gameField.resize(gameFieldHeight);
    for(int i = 0; i < gameField.size(); ++i)
    {
        gameField[i].resize(gameFieldWidth);
        for(int j = 0; j < gameField[i].size(); ++j)
        {
            gameField[i][j] = new QLabel;
            gameField[i][j]->setFixedSize(60,60);
            gameFieldLayout->addWidget(gameField[i][j], i, j);
        }
    }
}

void MaciLaciWidget::gameFieldImageChanged(int x, int y, Platforms changeTo)
{
    switch( changeTo )
    {
        case PLAYER: gameField[x][y]->setPixmap(QPixmap(":/images/macilaci.jpg"));
            break;
        case RANGER: gameField[x][y]->setPixmap(QPixmap(":/images/ranger.jpg"));
            break;
        case PICKNICKBASKET: gameField[x][y]->setPixmap(QPixmap(":/images/picknickbasket.jpg"));
            break;
        case GRASS:  gameField[x][y]->setPixmap(QPixmap(":/images/grass.jpg"));
            break;
        case TREE:   gameField[x][y]->setPixmap(QPixmap(":/images/tree.jpg"));
            break;
        case ROCK:   gameField[x][y]->setPixmap(QPixmap(":/images/rock.jpg"));
            break;
    }
}

