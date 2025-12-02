#include "chatController.h"


ChatController::ChatController(QObject *parent)
    : QObject(parent)
{

}

void ChatController::sendMessage(const QString &text)
{
    emit newMessage(text);
}
