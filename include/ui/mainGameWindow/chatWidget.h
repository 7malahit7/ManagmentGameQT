#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
    void displayMessage(const QString& text, bool isSystem);

signals:
    void sendMessage(const QString &text);
};
