#include "mainController.h"

#include "menuWidget.h"
#include "mainGameScreen.h"

#include "serverController.h"
#include "clientController.h"
#include "chatController.h"

#include <QDebug>

MainController::MainController(GameModel* model, QObject* parent)
    : QObject(parent),
    m_model(model),
    m_network(nullptr),
    m_chatController(nullptr),
    m_menu(nullptr),
    m_gameScreen(nullptr)
{
    m_menu = new MenuWidget();
    m_gameScreen = new MainGameScreen();

    // ===== MENU → CONTROLLER =====
    connect(m_menu, &MenuWidget::startGameClicked, this, [this]() {
        const QString name = m_menu->getName();
        startServer(name);
    });

    connect(m_menu, &MenuWidget::connectClicked, this, [this]() {
        const QString name = m_menu->getName();
        const QString ip   = m_menu->getIp();
        connectToServer(name, ip);
    });
}

MenuWidget* MainController::menu() const
{
    return m_menu;
}

MainGameScreen* MainController::gameScreen() const
{
    return m_gameScreen;
}

// =======================
// Server
// =======================

void MainController::startServer(const QString& playerName)
{
    qDebug() << "[] Starting as server";
    if (m_network)
        return;

    prepareLocalPlayer(playerName, true);
    auto& player = m_model->localPlayer();

    auto* server = new ServerController(&player, this);
    m_network = server;

    setupChat(
        true,
        &NetworkController::broadcast
        );

    connect(
        server,
        &ServerController::updatePlayers,
        this,
        &MainController::updatePlayersOnScreen,
        Qt::UniqueConnection
        );

    server->emitInitialPlayers();
    emit gameScreenRequested();
}

// =======================
// Client
// =======================

void MainController::connectToServer(
    const QString& playerName,
    const QString& host
    )
{
    qDebug() << "[] Starting as client";
    if (m_network) {
        qDebug() << "[MainController] Already connected or connecting";
        return;
    }

    prepareLocalPlayer(playerName, false);
    auto& player = m_model->localPlayer();

    auto* client = new ClientController(&player, host, this);
    m_network = client;

    setupChat(
        false,
        &NetworkController::sendChatMessage
        );

    connect(
        client,
        &ClientController::updatePlayers,
        this,
        &MainController::updatePlayersOnScreen,
        Qt::UniqueConnection
        );

    client->connectToServer();
    emit gameScreenRequested();
}

// =======================
// Helpers
// =======================

void MainController::prepareLocalPlayer(const QString& name, bool isServer)
{
    auto& player = m_model->localPlayer();
    player.setName(name);
    player.setIsServer(isServer);
}

void MainController::setupChat(
    bool isServer,
    void (NetworkController::*sendFunc)(const QJsonDocument&, MessageKind)
    )
{
    auto& player = m_model->localPlayer();

    m_chatController = new ChatController(
        player.getId(),
        player.getName(),
        isServer,
        this
        );

    connect(
        m_network,
        &NetworkController::sendMessageToChatController,
        m_chatController,
        &ChatController::onNetworkMessage
        );

    m_gameScreen->setChatController(m_chatController);

    connect(
        m_chatController,
        &ChatController::sendMessageToNetwork,
        m_network,
        sendFunc
        );
}

// =======================
// Players sync
// =======================

void MainController::updatePlayersOnScreen(
    const QVector<PlayerModel>& players
    )
{
    qDebug() << "[MainController] updatePlayersOnScreen, count =" << players.size();

    m_model->setPlayers(players);
    m_gameScreen->getLeftBarWidget()->updatePlayers(players);
}
