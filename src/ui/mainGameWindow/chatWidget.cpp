#include "mainGameWindow/chatWidget.h"

ChatWidget::ChatWidget(QWidget *parent)
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


    connect(sendMessageButton, &QPushButton::clicked, this, &ChatWidget::sendButtonClicked);
    connect(inputChat, &QLineEdit::returnPressed, this, &ChatWidget::sendButtonClicked);
}

void ChatWidget::sendButtonClicked()
{
    QString text = inputChat->text();
    if(!text.trimmed().isEmpty())
    {
        emit sendMessage(text);
        inputChat->clear();
    }
}

void ChatWidget::displayMessage(const QString &text)
{
    QString textForSend;
    textForSend += "<span style='color: yellow'>Игрок: </span>" + text;
    allChat->append(textForSend);
}
