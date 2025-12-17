#include "mainGameScreen.h"

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


}

void MainGameScreen::setChatController(ChatController* controller)
{
    if(controller && chatWidget)
    {
        // Локальные сообщения → ChatController
        QObject::connect(chatWidget, &ChatWidget::sendMessage, controller, &ChatController::sendChatMessage, Qt::UniqueConnection);
        // Сообщения из сети → ChatWidget
        QObject::connect(controller, &ChatController::newMessageFromNetwork, chatWidget, &ChatWidget::displayMessage, Qt::UniqueConnection);
    }
}
