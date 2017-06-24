#pragma once

#include <QWidget>
#include "Core/ICore.hpp"

namespace Ui
{
    class StoryNavigationWidget;
}

class StoryNodeItem;

class StoryNavigationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StoryNavigationWidget(QWidget* parent = Q_NULLPTR);
    virtual ~StoryNavigationWidget();

    void setCore(ICorePtr core);

private slots:
    void slotStoryStateChanged(bool state);
    void slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode);

private:
    void initConnects();

    QScopedPointer<Ui::StoryNavigationWidget> m_ui;
    ICorePtr m_core;
};
