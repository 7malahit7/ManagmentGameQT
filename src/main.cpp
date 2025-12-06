#include <QApplication>
#include "mainWindow.h"
#include "mainController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainController* controller = new MainController();

    MainWindow window(controller);
    window.show();

    return app.exec();
}
