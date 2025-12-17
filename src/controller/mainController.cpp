#include "mainController.h"
#include "clientController.h"
#include "serverController.h"
#include <QDebug>

MainController::MainController(QObject* parent) : QObject(parent)
{
    session = new GameSession(this);
    menu = new MenuWidget();
    mainGameScreen = new MainGameScreen();


    session->getLocalPlayer()->setName(menu->getName().isEmpty() ? "Player" : menu->getName());

    //хост
    connect(menu, &MenuWidget::startGameClicked, this, [this](){
        session->getLocalPlayer()->setIsServer(true);
        session->getLocalPlayer()->setId(1);

        session->startGameAsHost(true);

        auto net = session->getNetwork();
        if(net){
            initChatController(true);
            connect(chatController, &ChatController::sendMessageToNetwork,
                    static_cast<ServerController*>(net), &ServerController::broadcast,
                    Qt::UniqueConnection);

            connect(static_cast<ServerController*>(net), &ServerController::messageReceived,
                    chatController, &ChatController::onNetworkMessage,
                    Qt::UniqueConnection);
            emit gameScreenRequested();
        }

    });

    //клиент
    connect(menu, &MenuWidget::connectClicked, this, [this](){
        session->getLocalPlayer()->setIsServer(false);
        session->getLocalPlayer()->setId(2);
        QString ip = menu->getIp();
        session->connectToGame(ip);
        auto net = session->getNetwork();

        if(net){
            connect(static_cast<ClientController*>(net), &ClientController::connected, this, [this, net](){
                initChatController(false);
                connect(chatController, &ChatController::sendMessageToNetwork,
                        static_cast<ClientController*>(net), &ClientController::sendChatMessage,
                        Qt::UniqueConnection);

                connect(static_cast<ClientController*>(net), &ClientController::messageReceived,
                        chatController, &ChatController::onNetworkMessage,
                        Qt::UniqueConnection);
            });
            emit gameScreenRequested();
        }

    });

}

void MainController::initChatController(bool isServer)
{
    chatController = new ChatController(session->getLocalPlayer()->getId(),
                                        session->getLocalPlayer()->getName(),
                                        isServer, this);
    mainGameScreen->setChatController(chatController);

    connect(mainGameScreen->getChatWidget(), &ChatWidget::sendMessage,
            chatController, &ChatController::sendChatMessage,
            Qt::UniqueConnection);

    connect(chatController, &ChatController::newMessageFromNetwork,
            mainGameScreen->getChatWidget(), &ChatWidget::displayMessage,
            Qt::UniqueConnection);
}



