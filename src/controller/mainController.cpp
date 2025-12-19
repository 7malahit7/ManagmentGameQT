#include "mainController.h"

#include "menuWidget.h"
#include "mainGameScreen.h"

MainController::MainController(GameModel* model, QObject* parent)
    : QObject(parent),
    m_model(model),
    m_network(nullptr),
    m_menu(nullptr),
    m_gameScreen(nullptr)
{
    m_menu = new MenuWidget();
    m_gameScreen = new MainGameScreen();
}

MenuWidget* MainController::menu() const
{
    return m_menu;
}

MainGameScreen* MainController::gameScreen() const
{
    return m_gameScreen;
}
