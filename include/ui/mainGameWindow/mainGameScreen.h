#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

#include "gameModel.h"
#include "headerWidget.h"
#include "leftBarWidget.h"
#include "chatWidget.h"
#include "chatController.h"

class MainGameScreen : public QWidget
{
    Q_OBJECT
signals:
    void productionDecisionRequested(int esmAmount);
    void readyClicked();

public slots:
    void onMakeProductionDecision(int esmAmount);


public:
    explicit MainGameScreen(QWidget* parent = nullptr);

    void setChatController(ChatController* controller);
    ChatWidget* getChatWidget() const { return chatWidget; }
    LeftBarWidget *getLeftBarWidget() const;
    void setMonth(int month);
    void setState(GameState state);
    void setMarket(const MarketState& market);
    void setReadyEnabled(bool enabled);

private:
    QVBoxLayout* mainLayout;
    HeaderWidget* headerWidget;
    LeftBarWidget* leftBarWidget;
    ChatWidget* chatWidget;
    QPushButton* readyButton;
    QHBoxLayout* leftBarAndChatLayout;

};
