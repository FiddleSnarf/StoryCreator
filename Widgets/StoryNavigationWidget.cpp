#include "StoryNavigationWidget.hpp"
#include "ui_StoryNavigationWidget.h"
#include "./StoryManager.hpp"

StoryNavigationWidget::StoryNavigationWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::StoryNavigationWidget)
{
    m_ui->setupUi(this);
}

StoryNavigationWidget::~StoryNavigationWidget()
{

}

void StoryNavigationWidget::setCore(ICorePtr core)
{
    m_core = core;
    initConnects();
}

void StoryNavigationWidget::initConnects()
{
    if (!m_core)
        return;
}

void StoryNavigationWidget::slotStoryStateChanged(bool state)
{
    // TODO
}

void StoryNavigationWidget::slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode)
{
    // TODO
}
