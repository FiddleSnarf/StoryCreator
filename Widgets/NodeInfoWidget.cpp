#include "NodeInfoWidget.hpp"
#include "ui_NodeInfoWidget.h"

NodeInfoWidget::NodeInfoWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::NodeInfoWidget)
{
    m_ui->setupUi(this);
}

NodeInfoWidget::~NodeInfoWidget()
{

}
