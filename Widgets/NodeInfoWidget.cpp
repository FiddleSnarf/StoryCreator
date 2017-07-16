#include "NodeInfoWidget.hpp"
#include "Items/StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"
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
    // Добавил валидацию ввода для title (максимум 100 символов , русские-английские буквы ,.!?-)
    m_ui->nodeTitleLineEdit->setValidator(new QRegExpValidator(QRegExp(QString::fromLocal8Bit("[A-Za-zА-Яа-я1-9,.!?-]{1,100}")), this));

    blockUISignals();
    foreach(const QString& nodeTypeName, m_typesNodeCollector.getNodesNameList())
    {
        m_ui->nodeTypeComboBox->addItem(m_typesNodeCollector.getNodeTypeInfo(nodeTypeName).iconType, nodeTypeName);
    }

    connect(m_ui->nodeTitleLineEdit, &QLineEdit::textChanged, this, &NodeInfoWidget::slotUpdateNodeData);
    connect(m_ui->nodeTextEdit, &QTextEdit::textChanged, this, &NodeInfoWidget::slotUpdateNodeData);
    connect(m_ui->nodeTypeComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &NodeInfoWidget::slotUpdateNodeData);
    //connect(m_ui->actionTable); // TODO
    unblockUISignals();

    initHighlightParams();
}

void NodeInfoWidget::initHighlightParams()
{
    QTextCharFormat format;
    format.setBackground(StoryGUI::SEARCH_HIGHTLIGHT_COLOR);
    m_highlightSelection.format = format;
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

        updateSelectionSearchedText();
    }
    unblockUISignals();
}

void NodeInfoWidget::slotUpdateNodeData()
{
    if (m_currentNodeItem)
    {
        m_currentNodeItem->setNodeType(m_ui->nodeTypeComboBox->currentText());
        m_currentNodeItem->setNodeTitle(m_ui->nodeTitleLineEdit->text());
        m_currentNodeItem->setNodeText(m_ui->nodeTextEdit->toPlainText());
        // TODO обновление actions
    }
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

void NodeInfoWidget::slotNodeDeleted()
{
    // так как сейчас можно удалить только выделенный нод, то при удалении просто скрываем панель редактирования
    hide();
}

void NodeInfoWidget::slotSearchTextChanged(const QString& searchedText)
{
    m_searchedText = searchedText;
    updateSelectionSearchedText();
}

void NodeInfoWidget::updateSelectionSearchedText()
{
    if (m_ui->nodeTextEdit->toPlainText().isEmpty())
        return;

    QList<QTextEdit::ExtraSelection> listExSelection;
    QTextCursor cursor;
    do
    {
        cursor = QTextCursor(m_ui->nodeTextEdit->document()->find(m_searchedText, cursor, QTextDocument::FindCaseSensitively));
        m_highlightSelection.cursor = cursor;
        listExSelection << m_highlightSelection;
    }while(!cursor.isNull());
    m_ui->nodeTextEdit->setExtraSelections(listExSelection);
}
