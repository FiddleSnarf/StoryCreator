#include "NodeInfoWidget.hpp"
#include "Items/StoryNodeItemImpl.hpp"
#include "ui_NodeInfoWidget.h"

NodeInfoWidget::NodeInfoWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::NodeInfoWidget),
    m_currentNodeItem(nullptr)
{
    m_ui->setupUi(this);
    initialization();
}

NodeInfoWidget::~NodeInfoWidget()
{

}

void NodeInfoWidget::initialization()
{
    blockUISignals();
    foreach(const QString& nodeTypeName, m_typesNodeCollector.getNodesNameList())
    {
        m_ui->nodeTypeComboBox->addItem(m_typesNodeCollector.getNodeTypeInfo(nodeTypeName).iconType, nodeTypeName);
    }

    // TODO сделать валидацию ввода для title

    connect(m_ui->nodeTitleLineEdit, &QLineEdit::editingFinished, this, &NodeInfoWidget::slotUpdateNodeData);
    connect(m_ui->nodeTextEdit, &QTextEdit::textChanged, this, &NodeInfoWidget::slotUpdateNodeData);
    connect(m_ui->nodeTypeComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &NodeInfoWidget::slotUpdateNodeData);
    //connect(m_ui->actionTable); // TODO
    unblockUISignals();
}

void NodeInfoWidget::hideEvent(QHideEvent* event)
{
    m_currentNodeItem = nullptr;
    updateUI();
    QWidget::hideEvent(event);
}

void NodeInfoWidget::setCurrentNodeItem(StoryNodeItem* node)
{
    m_currentNodeItem = node;
    updateUI();
}

void NodeInfoWidget::updateUI()
{
    blockUISignals();
    if (!m_currentNodeItem)
    {
        // Очищаем данные на форме
        m_ui->nodeIdLabel->setText("---");
        m_ui->nodeTitleLineEdit->clear();
        m_ui->nodeTextEdit->clear();
        m_ui->nodeTypeComboBox->setCurrentIndex(-1);
        m_ui->actionTable->clear();
    }else
    {
        const StoryNode& nodeInfo = m_currentNodeItem->getNodeInfo();
        m_ui->nodeIdLabel->setText(QString::number(nodeInfo.getId())); // TODO выводить подпись, если это head node
        m_ui->nodeTitleLineEdit->setText(nodeInfo.getTitle());
        m_ui->nodeTextEdit->setText(nodeInfo.getText());
        for(int idx = 0; idx < m_ui->nodeTypeComboBox->count(); idx++)
        {
            if (m_ui->nodeTypeComboBox->itemText(idx) == nodeInfo.getType())
                m_ui->nodeTypeComboBox->setCurrentIndex(idx);
        }
        //m_ui->actionTable-> // TODO
    }
    unblockUISignals();
}

void NodeInfoWidget::slotUpdateNodeData()
{
    StoryNode& nodeInfo = m_currentNodeItem->getNodeInfo();
    nodeInfo.setType(m_ui->nodeTypeComboBox->currentText());
    nodeInfo.setTitle(m_ui->nodeTitleLineEdit->text());
    nodeInfo.setText(m_ui->nodeTextEdit->toPlainText());

    emit signalNodeInfoUpdated(nodeInfo.getId());
}

void NodeInfoWidget::blockUISignals()
{
    m_ui->nodeTitleLineEdit->blockSignals(true);
    m_ui->nodeTextEdit->blockSignals(true);
    m_ui->nodeTypeComboBox->blockSignals(true);
    m_ui->actionTable->blockSignals(true);
}

void NodeInfoWidget::unblockUISignals()
{
    m_ui->nodeTitleLineEdit->blockSignals(false);
    m_ui->nodeTextEdit->blockSignals(false);
    m_ui->nodeTypeComboBox->blockSignals(false);
    m_ui->actionTable->blockSignals(false);
}
