#include "menuWidget.h"

MenuWidget::MenuWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);

    QLabel* label = new QLabel("Введите имя и IP", this);
    mainLayout->addWidget(label);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Имя игрока");
    mainLayout->addWidget(nameInput);

    ipInput = new QLineEdit(this);
    ipInput->setPlaceholderText("IP (127.0.0.1)");
    mainLayout->addWidget(ipInput);

    startButton = new QPushButton("Создать игру", this);
    mainLayout->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &MenuWidget::startGameClicked);

    connectButton = new QPushButton("Подключиться", this);
    mainLayout->addWidget(connectButton);
    connect(connectButton, &QPushButton::clicked, this, &MenuWidget::connectClicked);
}
