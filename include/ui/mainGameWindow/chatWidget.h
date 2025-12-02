#pragma once

#include<QWidget>
#include<QTextEdit>
#include<QLineEdit>
#include<QPushButton>
#include<QBoxLayout>
#include<QString>

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
private slots:
    void sendButtonClicked();
public slots:
    void displayMessage(const QString& text);
signals:
    void sendMessage(const QString& text);
};
