#include <QApplication>

#include "mainController.h"
#include "mainWindow.h"
#include "gameModel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameModel model;
    MainController controller(&model);

    MainWindow window(&controller);
    window.show();

    return app.exec();
}
