#pragma once

#include <QWidget>

namespace Ui
{
    class SearchLineWidget;
}

class SearchLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchLineWidget(QWidget* parent = Q_NULLPTR);
    virtual ~SearchLineWidget();

private:
    QScopedPointer<Ui::SearchLineWidget> m_ui;
};
