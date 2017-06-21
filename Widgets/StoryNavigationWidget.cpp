#include "StoryNavigationWidget.hpp"
#include "ui_StoryNavigationWidget.h"

StoryNavigationWidget::StoryNavigationWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::StoryNavigationWidget)
{
    m_ui->setupUi(this);
}

StoryNavigationWidget::~StoryNavigationWidget()
{

}
