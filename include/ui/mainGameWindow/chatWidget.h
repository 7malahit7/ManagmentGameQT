#pragma once

#include<QWidget>
#include<QTextEdit>
#include<QLineEdit>
#include<QPushButton>
#include<QBoxLayout>
#include<QObject>

class ChatWidget : public QWidget
{
    Q_OBJECT
private:
    QTextEdit* allChat;
    QLineEdit* inputChat;
    QPushButton* sendMessageButton;
    QVBoxLayout* mainLayout;
    QHBoxLayout* inputLayout;
public:

    explicit ChatWidget(QWidget* parent = nullptr);
    ~ChatWidget() = default;
signals:

};
