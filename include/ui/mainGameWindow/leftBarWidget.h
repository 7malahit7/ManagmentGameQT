#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "playerModel.h"
#include "playerWidget.h"

class LeftBarWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* playersLabel;
    QVBoxLayout* mainLayout;
    void addNewPlayer(quint8 id, const QString& name);
    void clearPlayers();
public:
    explicit LeftBarWidget(QWidget* parent = nullptr);

    void updatePlayers(const QVector<PlayerModel> &players);
};
