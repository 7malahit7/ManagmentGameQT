#include <QApplication>
#include <QFont>

#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QVector<PlayerWidget*> players;
    players.append(new PlayerWidget(1, "Bob", 1, 10000, 5, 3));
    players.append(new PlayerWidget(2, "Tom", 2, 10000, 7, 4));
    players.append(new PlayerWidget(3, "Sam", 3, 15000, 10, 5));

    MainWindow window(players);

    return app.exec();
}
