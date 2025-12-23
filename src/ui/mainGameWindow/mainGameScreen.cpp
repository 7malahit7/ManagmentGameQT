#include "mainGameScreen.h"

LeftBarWidget *MainGameScreen::getLeftBarWidget() const
{
    return leftBarWidget;
}

void MainGameScreen::onMakeProductionDecision(int esmAmount)
{
    qDebug() << "[UI] Production decision requested, ESM =" << esmAmount;
    emit productionDecisionRequested(esmAmount);
}
MainGameScreen::MainGameScreen(QWidget* parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);

    headerWidget = new HeaderWidget("0", "Ожидание...", this);
    leftBarWidget = new LeftBarWidget(this);
    chatWidget = new ChatWidget(this);
    readyButton = new QPushButton("Готов", this);

    readyButton->setFixedHeight(80);

    leftBarAndChatLayout = new QHBoxLayout();
    leftBarWidget->setFixedWidth(400);
    leftBarAndChatLayout->addWidget(leftBarWidget);
    leftBarAndChatLayout->addSpacing(30);
    leftBarAndChatLayout->addWidget(chatWidget, 1);

    mainLayout->addWidget(headerWidget);
    mainLayout->addLayout(leftBarAndChatLayout);
    mainLayout->addWidget(readyButton);
    mainLayout->addSpacing(15);

    connect(readyButton, &QPushButton::clicked,
            this, &MainGameScreen::readyClicked);
}

void MainGameScreen::setChatController(ChatController* controller)
{
    if(controller && chatWidget)
    {
        QObject::connect(chatWidget, &ChatWidget::sendMessage, controller, &ChatController::sendChatMessage, Qt::UniqueConnection);
        QObject::connect(controller, &ChatController::newMessageFromNetwork, chatWidget, &ChatWidget::displayMessage, Qt::UniqueConnection);
    }
}

void MainGameScreen::setMonth(int month)
{
    qDebug() << "[MainGameScreen] Month =" << month;
}
void MainGameScreen::setState(GameState state)
{
    qDebug() << "[MainGameScreen] State =" << static_cast<int>(state);

}
void MainGameScreen::setMarket(const MarketState& market)
{
    qDebug() << "[MainGameScreen] Market:"
             << "level =" << market.level
             << "ESM supply =" << market.esmSupply
             << "ESM min price =" << market.esmMinPrice
             << "EGP demand =" << market.egpDemand
             << "EGP max price =" << market.egpMaxPrice;
}

void MainGameScreen::setReadyEnabled(bool enabled)
{
    readyButton->setEnabled(enabled);
}

