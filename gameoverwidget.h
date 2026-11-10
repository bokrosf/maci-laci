#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class GameOverWidget;
}

class GameOverWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::GameOverWidget *ui;

public:
    GameOverWidget(QString endingText, const QString &endingSound, QWidget *parent = nullptr);
    ~GameOverWidget();
};

#endif // GAMEOVERWIDGET_H
