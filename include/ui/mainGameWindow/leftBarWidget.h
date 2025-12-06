#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class LeftBarWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* playersLabel;
    QVBoxLayout* mainLayout;

public:
    explicit LeftBarWidget(QWidget* parent = nullptr);
};
