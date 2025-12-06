#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class MenuWidget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonsLayout;
    QLabel* menuLabel;
    QLineEdit* nameInput;
    QLineEdit* ipInput;
    QPushButton* connectButton;
    QPushButton* startButton;

public:
    explicit MenuWidget(QWidget* parent = nullptr);

    QString getName() const { return nameInput->text(); }
    QString getIp() const { return ipInput->text(); }

signals:
    void startGameClicked();
    void connectClicked();
};
