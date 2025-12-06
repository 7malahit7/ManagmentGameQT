#include "mainController.h"
#include "networkController.h"
#include <QDebug>

MainController::MainController(QObject* parent) : QObject(parent)
{
    session = new GameSession(this);
    menu = new MenuWidget();
    mainGameScreen = new MainGameScreen();

    bool isServerMode = true;
    if(!session->getLocalPlayer()->getIsServer())
        isServerMode = false;

    chatController = new ChatController(session->getLocalPlayer()->getId(), isServerMode, this);
    mainGameScreen->setChatController(chatController);

    connect(chatController, &ChatController::newMessage,
            mainGameScreen->getChatWidget(), &ChatWidget::displayMessage,
            Qt::UniqueConnection);

    connect(mainGameScreen->getChatWidget(), &ChatWidget::sendMessage,
            chatController, &ChatController::onLocalMessage,
            Qt::UniqueConnection);

    connect(menu, &MenuWidget::startGameClicked, this, [this](){
        session->startGame(true);

        auto net = session->getNetwork();
        if(net){
            connect(net, &NetworkController::messageReceived,
                    chatController, &ChatController::onNetworkMessage,
                    Qt::UniqueConnection);

            connect(chatController, &ChatController::sendMessage,
                    net, &NetworkController::sendMessageToServerOrBroadcast,
                    Qt::UniqueConnection);
        }

        emit gameScreenRequested();
    });

    // Клиент
    connect(menu, &MenuWidget::connectClicked, this, [this](){
        QString ip = menu->getIp();
        session->startNetworkGame(ip);

        auto net = session->getNetwork();
        if(net){
            connect(net, &NetworkController::connected, this, [this, net](){
                qDebug() << "[MainController] Client connected to server";

                connect(net, &NetworkController::messageReceived,
                        chatController, &ChatController::onNetworkMessage,
                        Qt::UniqueConnection);

                connect(chatController, &ChatController::sendMessage,
                        net, &NetworkController::sendMessageToServerOrBroadcast,
                        Qt::UniqueConnection);

                emit gameScreenRequested();
            });
        }
    });
}
