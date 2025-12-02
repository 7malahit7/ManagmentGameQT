#include "mainGameWindow/mainGameScreen.h"


MainGameWindow::MainGameWindow(const QVector<PlayerWidget*>& players, QWidget *parent)
    : QWidget(parent)
{
    chatController = new ChatController;

    headerWidget = new HeaderWidget("0", "Ожидание...",this);
    leftBarWidget = new LeftBarWidget(players,this);
    chatWidget = new ChatWidget(this);

    mainLayout = new QVBoxLayout(this);
    leftBarAndChatLayout = new QHBoxLayout();

    ReadyButton = new QPushButton("Готов");


    leftBarAndChatLayout->addWidget(leftBarWidget);
    leftBarAndChatLayout->addSpacing(30);
    leftBarAndChatLayout->addWidget(chatWidget);

    leftBarWidget->setFixedWidth(400);

    ReadyButton->setFixedHeight(80);

    mainLayout->addWidget(headerWidget);
    mainLayout->addLayout(leftBarAndChatLayout);
    mainLayout->addWidget(ReadyButton);

    mainLayout->addSpacing(15);


    connect(chatWidget, &ChatWidget::sendMessage, chatController,&ChatController::sendMessage);
    connect(chatController, &ChatController::newMessage, chatWidget, &ChatWidget::displayMessage);
}


