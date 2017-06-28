#include "StoryNavigationWidget.hpp"
#include "ui_StoryNavigationWidget.h"
#include "./StoryManager.hpp"

StoryNavigationWidget::StoryNavigationWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui::StoryNavigationWidget),
    m_currentSelectedNodeId(-1)
{
    m_ui->setupUi(this);
    initialization();
}

StoryNavigationWidget::~StoryNavigationWidget()
{

}

void StoryNavigationWidget::initialization()
{
    // Разрешаем выделение только одного элемента
    m_ui->nodesNavigationTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Разрешаем выделение построчно
    m_ui->nodesNavigationTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Растягиваем последний столбец до максимума
    m_ui->nodesNavigationTable->horizontalHeader()->setStretchLastSection(true);

    m_ui->nodesNavigationTable->setColumnCount(enCountColumn);
    QStringList labels;
    labels << tr("Node ID") << tr("Title");
    m_ui->nodesNavigationTable->setHorizontalHeaderLabels(labels);

    m_ui->nodesNavigationTable->setColumnWidth(enIdColumn, 60);
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

    connect(m_core->getStoryManager().data(), &StoryManager::signalStoryStateChanged, this, &StoryNavigationWidget::slotStoryStateChanged);
    connect(m_core->getStoryManager().data(), &StoryManager::signalItemSelectedChanged, this, &StoryNavigationWidget::slotItemSelectedChanged);
    connect(m_ui->nodesNavigationTable, &QTableWidget::itemClicked, this, &StoryNavigationWidget::slotNavigationItemClicked);
}

void StoryNavigationWidget::slotStoryStateChanged(bool state)
{
    StoryManagerPtr storyManager = m_core->getStoryManager();
    if (state)
    {
        foreach(const StoryNodeItemPtr& node ,storyManager->getStoryNodeList())
        {
            const StoryNode& storyNodeInfo = node->getNodeInfo();
            const int nodeId = storyNodeInfo.getId();
            const QString nodeTitle = storyNodeInfo.getTitle();
            const bool isValid = storyNodeInfo.isValid();
            m_nodeRowMap[nodeId] = NodeRowInfo(nodeTitle, isValid);
        }
        refillNodeTable();
    }else
    {
        m_nodeRowMap.clear();
        clearNodeTable();
    }
}

void StoryNavigationWidget::slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode)
{
    const int selectedId = selectedNode->getNodeInfo().getId();
    if (state && m_currentSelectedNodeId == selectedId)
        return;

    clearSelection();
    if (state)
    {
        for (int i = 0; i < m_ui->nodesNavigationTable->rowCount(); i++)
        {
            QTableWidgetItem* idItem = m_ui->nodesNavigationTable->item(i, enIdColumn);
            if (idItem->text().toInt() == selectedId)
            {
                m_currentSelectedNodeId = selectedId;
                idItem->setSelected(true);
                m_ui->nodesNavigationTable->item(i, enTitleColumn)->setSelected(true);
                m_ui->nodesNavigationTable->scrollToItem(idItem, QAbstractItemView::PositionAtCenter);
                return;
            }
        }
    }
}

void StoryNavigationWidget::slotNavigationItemClicked(QTableWidgetItem* item)
{
    const int nodeID = (item->column() == enIdColumn) ? item->text().toInt() : m_ui->nodesNavigationTable->item(item->row(), enIdColumn)->text().toInt();
    m_currentSelectedNodeId = nodeID;
    m_core->getStoryManager()->selectNodeForID(nodeID, true);
}

void StoryNavigationWidget::clearSelection()
{
    QList<QTableWidgetItem*> selectedItems = m_ui->nodesNavigationTable->selectedItems();
    foreach(QTableWidgetItem* item, selectedItems)
    {
        item->setSelected(false);
    }
}

void StoryNavigationWidget::slotUserAddedNode(int nodeID)
{
    StoryNodeItemPtr nodeItem = m_core->getStoryManager()->getNodeItemForID(nodeID);
    if (nodeItem)
    {
        const StoryNode& storyNodeInfo = nodeItem->getNodeInfo();
        appendNodeRow(storyNodeInfo.getId(), storyNodeInfo.getTitle(), storyNodeInfo.isValid());
    }
}

void StoryNavigationWidget::slotUserDeletedNode(int nodeID)
{
    // TODO
}

void StoryNavigationWidget::appendNodeRow(int nodeId, const QString& nodeTitle, bool nodeIsValid)
{
    m_nodeRowMap[nodeId] = NodeRowInfo(nodeTitle, nodeIsValid);
    const int lastRowIndex = m_ui->nodesNavigationTable->rowCount();
    m_ui->nodesNavigationTable->setRowCount(lastRowIndex + 1);
    if (lastRowIndex > 0)
    {
        QTableWidgetItem* preItem = m_ui->nodesNavigationTable->item(lastRowIndex - 1, enIdColumn);
        const int preItemId = preItem->text().toInt();
        if (nodeId < preItemId)
        {
            refillNodeTable();
            return;
        }
    }

    QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(nodeId));
    idItem->setFont(QFont("Arial", 14, QFont::Bold,QFont::StyleItalic));
    idItem->setTextAlignment(Qt::AlignCenter);
    idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);

    QTableWidgetItem* titleItem = new QTableWidgetItem(nodeTitle);
    titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
    //titleItem->setFont(QFont("Arial", 14, QFont::Bold,QFont::StyleItalic)); // TODO подобрать font

    const QColor backgroundColor = getColorForNodeRow(nodeIsValid);
    idItem->setBackgroundColor(backgroundColor);
    titleItem->setBackgroundColor(backgroundColor);

    m_ui->nodesNavigationTable->setItem(lastRowIndex, enIdColumn, idItem);
    m_ui->nodesNavigationTable->setItem(lastRowIndex, enTitleColumn, titleItem);
}

void StoryNavigationWidget::refillNodeTable()
{
    clearNodeTable();
    for(NodeRowMap::const_iterator it = m_nodeRowMap.cbegin(); it != m_nodeRowMap.cend(); ++it)
    {
        appendNodeRow(it.key(), (*it).first, (*it).second);
    }
}

void StoryNavigationWidget::clearNodeTable()
{
    m_currentSelectedNodeId = -1;
    m_ui->nodesNavigationTable->clearContents();
    m_ui->nodesNavigationTable->setRowCount(0);
}

QColor StoryNavigationWidget::getColorForNodeRow(bool isValid) const
{
    return isValid ? QColor(Qt::green) : QColor(Qt::red); // TODO подобрать цвета
}
