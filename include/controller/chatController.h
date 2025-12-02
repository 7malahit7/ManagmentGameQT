#pragma once

#include<QObject>
#include<QString>

class ChatController : public QObject
{
    Q_OBJECT
public:
    ChatController(QObject* parent = nullptr);

public slots:
    void sendMessage(const QString& text);
signals:
    void newMessage(const QString& text);
};
