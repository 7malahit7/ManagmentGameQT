#include "chatWidget.h"

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    inputLayout = new QHBoxLayout();

    allChat = new QTextEdit();
    inputChat = new QLineEdit();

    sendMessageButton = new QPushButton("ОТПРАВИТЬ");

    inputChat->setFixedHeight(60);
    sendMessageButton->setFixedHeight(60);

    inputLayout->addWidget(inputChat);
    inputLayout->addWidget(sendMessageButton);
    inputLayout->setSpacing(5);

    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->addWidget(allChat, 1);
    mainLayout->addLayout(inputLayout, 0);

    allChat->setReadOnly(true);

    inputChat->setMaxLength(100);
    inputChat->setPlaceholderText("Введите сообщение...");



    connect(sendMessageButton, &QPushButton::clicked, this, [this](){
        QString text = inputChat->text();
        if(!text.isEmpty()){
            qDebug() << "[ChatWidget] send button pressed!";
            emit sendMessage(text);
            inputChat->clear();
        }
    });

    connect(inputChat, &QLineEdit::returnPressed, this, [this](){
        QString text = inputChat->text();
        if(!text.isEmpty()){
            qDebug() << "[ChatWidget] send button pressed!";
            emit sendMessage(text);
            inputChat->clear();
        }
    });
}

void ChatWidget::displayMessage(const QString &text)
{
    QString message;
    message += "> " + text;

    allChat->append(message);
}
