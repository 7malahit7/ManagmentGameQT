#pragma once

#include <QWidget>


#include "mainGameWindow/chatWidget.h"
#include "mainGameWindow/headerWidget.h"
#include "mainGameWindow/leftBarWidget.h"
#include "mainGameWindow/playerWidget.h"
#include "chatController.h"



class MainGameWindow : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* leftBarAndChatLayout;

    QVector<PlayerWidget*> playersWidgets;

    ChatWidget* chatWidget;
    HeaderWidget* headerWidget;
    LeftBarWidget* leftBarWidget;
    QPushButton* ReadyButton;

    ChatController *chatController;

public:

    explicit MainGameWindow(const QVector<PlayerWidget*>& players, QWidget *parent = nullptr);
    ~MainGameWindow() = default;
};


