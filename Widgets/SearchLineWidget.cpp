#include "SearchLineWidget.hpp"
#include "ui_SearchLineWidget.h"

SearchLineWidget::SearchLineWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::SearchLineWidget)
{
    m_ui->setupUi(this);
    m_ui->iconLabel->setPixmap(QIcon(":/other/Resources/search.png").pixmap(20,20));

    connect(m_ui->searchLine, &QLineEdit::textChanged, this, &SearchLineWidget::signalTextChanged);
}

SearchLineWidget::~SearchLineWidget()
{

}

QString SearchLineWidget::getText() const
{
    return m_ui->searchLine->text();
}

void SearchLineWidget::setText(const QString& text)
{
    m_ui->searchLine->setText(text);
}

void SearchLineWidget::setMaxLengthText(int length)
{
    m_ui->searchLine->setMaxLength(length);
}

void SearchLineWidget::clear()
{
    m_ui->searchLine->clear();
}

