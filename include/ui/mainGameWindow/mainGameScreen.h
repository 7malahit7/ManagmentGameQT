#ifndef MAINGAMESCREEN_H
#define MAINGAMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
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
    QPushButton* ReadyButton;

public:
    explicit MainGameScreen(QWidget* parent = nullptr);

    ChatWidget* getChatWidget() const { return chatWidget; }

    // Подключаем контроллер чата
    void setChatController(ChatController* controller);

signals:

};

#endif // MAINGAMESCREEN_H
