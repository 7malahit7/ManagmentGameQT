#include "menuWidget.h"

MenuWidget::MenuWidget(QWidget* parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    buttonsLayout = new QHBoxLayout();

    menuLabel = new QLabel("Меню", this);
    menuLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(menuLabel);
    mainLayout->addSpacing(100);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Введите ваше Имя...");
    nameInput->setFixedHeight(40);
    mainLayout->addWidget(nameInput);

    ipInput = new QLineEdit(this);
    ipInput->setPlaceholderText("Введите IP адрес...");
    ipInput->setFixedHeight(40);
    mainLayout->addWidget(ipInput);

    mainLayout->addSpacing(60);

    connectButton = new QPushButton("Подключиться", this);
    startButton = new QPushButton("Начать игру", this);

    connectButton->setFixedSize(150, 50);
    startButton->setFixedSize(150, 50);

    buttonsLayout->addWidget(connectButton);
    buttonsLayout->addWidget(startButton);
    mainLayout->addLayout(buttonsLayout);

    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setAlignment(Qt::AlignHCenter);

    connect(startButton, &QPushButton::clicked, this, &MenuWidget::startGameClicked);
    connect(connectButton, &QPushButton::clicked, this, &MenuWidget::connectClicked);
}
