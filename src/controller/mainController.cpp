#include "mainController.h"

#include "menuWidget.h"
#include "mainGameScreen.h"

#include "serverController.h"
#include "clientController.h"
#include "chatController.h"

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

    // ===== MENU → CONTROLLER (реальный API) =====
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
// Network
// =======================

void MainController::startServer(const QString& playerName)
{
    if (m_network) {
        qDebug() << "[MainController] Server already running";
        return;
    }
    auto& player = m_model->localPlayer();
    player.setName(playerName);
    player.setIsServer(true);

    auto* server = new ServerController(&player, this);
    m_network = server;

    m_chatController = new ChatController(
        player.getId(),
        player.getName(),
        true,
        this
        );

    // Network → Chat
    connect(
        m_network,
        &NetworkController::sendMessageToChatController,
        m_chatController,
        &ChatController::onNetworkMessage
        );

    emit gameScreenRequested();
}

void MainController::connectToServer(
    const QString& playerName,
    const QString& host
    )
{
    if (m_network) {
        qDebug() << "[MainController] Already connected or connecting";
        return;
    }
    auto& player = m_model->localPlayer();
    player.setName(playerName);
    player.setIsServer(false);

    auto* client = new ClientController(&player, host, this);
    m_network = client;

    m_chatController = new ChatController(
        player.getId(),
        player.getName(),
        false,
        this
        );

    // Network → Chat
    connect(
        m_network,
        &NetworkController::sendMessageToChatController,
        m_chatController,
        &ChatController::onNetworkMessage
        );

    emit gameScreenRequested();
}
