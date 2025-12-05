#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

class MenuWidget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    QPushButton* startButton;
    QPushButton* connectButton;
    QLineEdit* nameInput;
    QLineEdit* ipInput;

public:
    explicit MenuWidget(QWidget* parent = nullptr);

    QPushButton* getStartButton() const { return startButton; }
    QPushButton* getConnectButton() const { return connectButton; }
    QString getName() const { return nameInput->text(); }
    QString getIp() const { return ipInput->text(); }

signals:
    void startGameClicked();
    void connectClicked();
};
