#pragma once

#include <QWidget>

namespace Ui
{
    class StoryNavigationWidget;
}

class StoryNavigationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StoryNavigationWidget(QWidget* parent = Q_NULLPTR);
    virtual ~StoryNavigationWidget();

private:
    QScopedPointer<Ui::StoryNavigationWidget> m_ui;
};
