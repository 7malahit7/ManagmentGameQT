#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "headerWidget.h"
#include "leftBarWidget.h"
#include "chatWidget.h"
#include "chatController.h"

class MainGameScreen : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    HeaderWidget* headerWidget;
    LeftBarWidget* leftBarWidget;
    ChatWidget* chatWidget;
    QPushButton* readyButton;
    QHBoxLayout* leftBarAndChatLayout;

public:
    explicit MainGameScreen(QWidget* parent = nullptr);
    void setChatController(ChatController* controller);
    ChatWidget* getChatWidget() const { return chatWidget; }
    LeftBarWidget *getLeftBarWidget() const;
};
