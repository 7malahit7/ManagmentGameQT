#pragma once

#include<QWidget>
#include<QBoxLayout>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>

class MenuWidget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonsLayout;
    QPushButton* connectButton;
    QPushButton* startButton;
    QLineEdit* ipInput;
    QLineEdit* nameInput;
    QLabel* menuLabel;
public:
    explicit MenuWidget(QWidget* parent = nullptr);
    ~MenuWidget() = default;
private slots:
    void onStartGameClicked();

signals:
    void switchToGameScreen();
};
