#include <QSoundEffect>
#include <QLayout>
#include "gameoverwidget.h"
#include "ui_gameoverwidget.h"

GameOverWidget::GameOverWidget(QString endingText, const QString &endingSound, QWidget *parent) :
    QWidget(parent), ui(new Ui::GameOverWidget)
{
    ui->setupUi(this);

    setWindowTitle("Játék vége");
    QSoundEffect sound;
    sound.setSource(QUrl::fromLocalFile(endingSound));
    sound.play();
    setAttribute(Qt::WA_ShowModal); //csak ezt az egy ablakot engedi vezérelni és nem lehet átváltani másikra, amíg ezt be nem zárjuk
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(361, 150);
    ui->gameEndingLabel->setText(endingText);
    connect(ui->OK_Button, SIGNAL(clicked(bool)), this, SLOT(close()) );
}

GameOverWidget::~GameOverWidget()
{
    delete ui;
}
