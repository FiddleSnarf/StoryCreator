#include "SearchLineWidget.hpp"
#include "ui_SearchLineWidget.h"

SearchLineWidget::SearchLineWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::SearchLineWidget)
{
    m_ui->setupUi(this);
    m_ui->iconLabel->setPixmap(QIcon(":/other/Resources/search.png").pixmap(20,20));
}

SearchLineWidget::~SearchLineWidget()
{

}

