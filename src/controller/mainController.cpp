#include "mainController.h"

#include "menuWidget.h"
#include "mainGameScreen.h"

#include "serverController.h"
#include "clientController.h"
#include "chatController.h"

#include <QJsonObject>
#include <QDebug>

MainController::MainController(GameModel* model, QObject* parent)
    : QObject(parent),
    m_model(model)
{
    m_menu = new MenuWidget();
    m_gameScreen = new MainGameScreen();

    connect(m_menu, &MenuWidget::startGameClicked, this, [this]() {
        startServer(m_menu->getName());
    });

    connect(m_menu, &MenuWidget::connectClicked, this, [this]() {
        connectToServer(m_menu->getName(), m_menu->getIp());
    });

    connect(m_model, &GameModel::monthChanged,
            this, &MainController::onMonthChanged);

    connect(m_model, &GameModel::stateChanged,
            this, &MainController::onStateChanged);

    connect(m_model, &GameModel::marketChanged,
            this, &MainController::onMarketChanged);

    connect(m_gameScreen, &MainGameScreen::readyClicked,
            this, &MainController::onReadyClicked);

    connect(
        m_gameScreen,
        &MainGameScreen::readyClicked,
        this,
        &MainController::onReadyClicked);


}

MenuWidget* MainController::menu() const
{
    return m_menu;
}

MainGameScreen* MainController::gameScreen() const
{
    return m_gameScreen;
}

void MainController::startServer(const QString& playerName)
{
    if (m_network)
        return;

    qDebug() << "[MainController] Starting as SERVER";

    prepareLocalPlayer(playerName, true);

    setupServerNetwork(m_model->localPlayer());
    setupChat(true, &NetworkController::broadcast);

    showGameScreen();
}

void MainController::connectToServer(
    const QString& playerName,
    const QString& host)
{
    if (m_network)
        return;

    qDebug() << "[MainController] Starting as CLIENT";

    prepareLocalPlayer(playerName, false);

    setupClientNetwork(m_model->localPlayer(), host);
    setupChat(false, &NetworkController::sendChatMessage);

    showGameScreen();
}

void MainController::setupServerNetwork(PlayerModel& player)
{
    auto* server = new ServerController(&player, this);
    m_network = server;

    connect(server, &ServerController::updatePlayers,
            this, &MainController::updatePlayersOnScreen,
            Qt::UniqueConnection);

    server->emitInitialPlayers();
}

void MainController::setupClientNetwork(PlayerModel& player, const QString& host)
{
    auto* client = new ClientController(&player, host, this);
    m_network = client;

    connect(client, &ClientController::updatePlayers,
            this, &MainController::updatePlayersOnScreen,
            Qt::UniqueConnection);

    client->connectToServer();
}

void MainController::setupChat(
    bool isServer,
    void (NetworkController::*sendFunc)(const QJsonDocument&, MessageKind))
{
    auto& player = m_model->localPlayer();

    m_chatController = new ChatController(
        player.getId(),
        player.getName(),
        isServer,
        this);

    connect(m_network,
            &NetworkController::sendMessageToChatController,
            m_chatController,
            &ChatController::onNetworkMessage);

    connect(m_chatController,
            &ChatController::sendMessageToNetwork,
            m_network,
            sendFunc);

    m_gameScreen->setChatController(m_chatController);
}

void MainController::onReadyClicked()
{
    QJsonObject obj;
    obj["type"] = "player_ready";
    obj["playerId"] = m_model->localPlayer().getId();

    m_network->sendReady(QJsonDocument(obj));
}

void MainController::prepareLocalPlayer(const QString& name, bool isServer)
{
    auto& player = m_model->localPlayer();
    player.setName(name);
    player.setIsServer(isServer);
}

void MainController::showGameScreen()
{
    emit gameScreenRequested();
}

void MainController::updatePlayersOnScreen(
    const QVector<PlayerModel>& players)
{
    qDebug() << "[MainController] updatePlayersOnScreen, count =" << players.size();

    m_model->setPlayers(players);
    m_gameScreen->getLeftBarWidget()->updatePlayers(players);
}

void MainController::onMonthChanged(int month)
{
    qDebug() << "[UI] Month changed to" << month;
    m_gameScreen->setMonth(month);
}

void MainController::onStateChanged(GameState state)
{
    qDebug() << "[UI] State changed to" << static_cast<int>(state);
    m_gameScreen->setState(state);
}

void MainController::onMarketChanged(const MarketState& market)
{
    m_gameScreen->setMarket(market);
}

void MainController::onPlayerReadyFromNetwork(int playerId)
{
    m_model->setPlayerReady(playerId);
    m_model->tryAdvancePhase();
}
