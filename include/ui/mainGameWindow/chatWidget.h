#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

class ChatWidget : public QWidget
{
    Q_OBJECT
private:
    QTextEdit* chatView;
    QLineEdit* input;

public:
    explicit ChatWidget(QWidget* parent = nullptr);

signals:
    void sendMessage(const QString &text);

public slots:
    void displayMessage(const QString &text);
};
