#include "chatWidget.h"
#include <QKeyEvent>

ChatWidget::ChatWidget(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    chatView = new QTextEdit(this);
    chatView->setReadOnly(true);

    input = new QLineEdit(this);
    layout->addWidget(chatView);
    layout->addWidget(input);

    connect(input, &QLineEdit::returnPressed, this, [this]() {
        QString text = input->text();
        if(!text.isEmpty())
        {
            emit sendMessage(text);
            input->clear();
        }
    });
}

void ChatWidget::displayMessage(const QString &text)
{
    chatView->append(text);
}
