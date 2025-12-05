#include "mainGameScreen.h"

MainGameScreen::MainGameScreen(QWidget* parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);

    headerWidget = new HeaderWidget("January", "Phase1", this);
    leftBarWidget = new LeftBarWidget(this);
    chatWidget = new ChatWidget(this);
    ReadyButton = new QPushButton("Ready", this);

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(leftBarWidget);
    mainLayout->addWidget(chatWidget);
    mainLayout->addWidget(ReadyButton);
}

void MainGameScreen::setChatController(ChatController* controller)
{
    if(controller && chatWidget)
    {
        // Локальные сообщения → ChatController
        QObject::connect(chatWidget, &ChatWidget::sendMessage,
                         controller, &ChatController::onLocalMessage,
                         Qt::UniqueConnection);

        // Сообщения из сети → ChatWidget
        QObject::connect(controller, &ChatController::newMessage,
                         chatWidget, &ChatWidget::displayMessage,
                         Qt::UniqueConnection);
    }
}
