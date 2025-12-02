#include "menu/menu.h"


MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    buttonsLayout = new QHBoxLayout();
    menuLabel = new QLabel("Меню", this);
    nameInput = new QLineEdit(this);
    ipInput = new QLineEdit(this);
    connectButton = new QPushButton("Подключиться", this);
    startButton = new QPushButton("Начать игру",this);

    buttonsLayout->addWidget(connectButton);
    buttonsLayout->addWidget(startButton);


    mainLayout->addWidget(menuLabel);
    mainLayout->addSpacing(100);
    mainLayout->addWidget(nameInput);
    mainLayout->addWidget(ipInput);
    mainLayout->addSpacing(60);
    mainLayout->addLayout(buttonsLayout);

    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setAlignment(Qt::AlignHCenter);

    menuLabel->setAlignment(Qt::AlignHCenter);

    connectButton->setFixedSize(150,50);
    startButton->setFixedSize(150,50);

    ipInput->setFixedHeight(40);
    nameInput->setFixedHeight(40);

    ipInput->setPlaceholderText("Введите IP адрес...");
    nameInput->setPlaceholderText("Введите ваше Имя...");

    connect(startButton, &QPushButton::clicked,this, &MenuWidget::onStartGameClicked);
}

void MenuWidget::onStartGameClicked()
{
    nameInput->setEnabled(false);
    ipInput->setEnabled(false);
    emit switchToGameScreen();
}
