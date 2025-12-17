#include "networkController.h"
#include <QDebug>

NetworkController::NetworkController(QObject *parent)
    : QObject(parent)
{
    connect(this, &NetworkController::sendToNetwork, this, &NetworkController::onDataReceived);
}
