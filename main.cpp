#include <QApplication>
#include "macilaciwidget.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);    
    MaciLaciWidget w;
    w.show();

    return a.exec();
}
