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

void StoryNavigationWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
        emit signalDeleteKeyPressed();
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

    StoryManagerPtr storyManager = m_core->getStoryManager();
    connect(storyManager.data(), &StoryManager::signalStoryStateChanged, this, &StoryNavigationWidget::slotStoryStateChanged);
    connect(storyManager.data(), &StoryManager::signalItemSelectedChanged, this, &StoryNavigationWidget::slotItemSelectedChanged);
    connect(storyManager.data(), &StoryManager::signalStoryNodeAdded, this, &StoryNavigationWidget::slotUserAddedNode);
    connect(storyManager.data(), &StoryManager::signalStoryNodeDeleted, this, &StoryNavigationWidget::slotUserDeletedNode);
    connect(storyManager.data(), &StoryManager::signalDataNodeChanged, this, &StoryNavigationWidget::slotUserEditedNode);

    connect(m_ui->nodesNavigationTable, &QTableWidget::itemClicked, this, &StoryNavigationWidget::slotNavigationItemClicked);
    connect(m_ui->searchLine, &SearchLineWidget::signalTextChanged, this, &StoryNavigationWidget::slotSearchTextChanged);

    connect(this, &StoryNavigationWidget::signalDeleteKeyPressed, storyManager.data(), &StoryManager::slotDeleteSelectedNode);
}

void StoryNavigationWidget::slotStoryStateChanged(bool state)
{
    if (!state)
    {
        m_nodeNaviInfoMap.clear();
        clearNodeTable();
    }
}

void StoryNavigationWidget::slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode)
{       
    if (state)
    {
        const int selectedId = selectedNode->getNodeInfo().getId();
        if (m_currentSelectedNodeId == selectedId)
            return;
        else
            clearSelection();

        for (int rowIdx = 0; rowIdx < m_ui->nodesNavigationTable->rowCount(); rowIdx++)
        {
            QTableWidgetItem* idItem = m_ui->nodesNavigationTable->item(rowIdx, enIdColumn);
            if (idItem->text().toInt() == selectedId)
            {
                m_currentSelectedNodeId = selectedId;
                idItem->setSelected(true);
                m_ui->nodesNavigationTable->item(rowIdx, enTitleColumn)->setSelected(true);
                m_ui->nodesNavigationTable->scrollToItem(idItem, QAbstractItemView::PositionAtCenter);
                return;
            }
        }
    }
    else
        clearSelection();
}

void StoryNavigationWidget::slotNavigationItemClicked(QTableWidgetItem* item)
{
    const int nodeID = (item->column() == enIdColumn) ? item->text().toInt() : m_ui->nodesNavigationTable->item(item->row(), enIdColumn)->text().toInt();
    m_currentSelectedNodeId = nodeID;
    m_core->getStoryManager()->selectNodeForID(nodeID, true);
}

void StoryNavigationWidget::clearSelection()
{
    m_currentSelectedNodeId = -1;
    QList<QTableWidgetItem*> selectedItems = m_ui->nodesNavigationTable->selectedItems();
    foreach(QTableWidgetItem* item, selectedItems)
    {
        item->setSelected(false);
    }
}

void StoryNavigationWidget::slotUserAddedNode(StoryNodeItem* addedNode)
{
    const StoryNode& storyNodeInfo = addedNode->getNodeInfo();

    NodeNavigationInfo naviInfo;
    naviInfo.title = storyNodeInfo.getTitle();
    naviInfo.text = storyNodeInfo.getText();
    naviInfo.nodeItemPtr = addedNode;
    m_nodeNaviInfoMap[storyNodeInfo.getId()] = naviInfo;

    // Теперь проверим если ноды пошли не по порядку, перезаполним таблицу, иначе просто добавим новую строку
    const int lastRowIndex = m_ui->nodesNavigationTable->rowCount();
    if (lastRowIndex != 0 && m_ui->nodesNavigationTable->item(lastRowIndex - 1, enIdColumn)->text().toInt() > addedNode->getNodeInfo().getId())
        refillNodeTable();
    else
        appendNodeRow(storyNodeInfo.getId(), storyNodeInfo.getTitle(), storyNodeInfo.isValid());
}

void StoryNavigationWidget::slotUserDeletedNode(int nodeID)
{
    clearSelection();
    m_nodeNaviInfoMap.remove(nodeID);
    refillNodeTable();
}

void StoryNavigationWidget::slotUserEditedNode(int nodeID)
{
    NodeNavigationInfo& nodeNaviInfo = m_nodeNaviInfoMap[nodeID];
    nodeNaviInfo.updateInfo();
    for (int rowIdx = 0; rowIdx < m_ui->nodesNavigationTable->rowCount(); rowIdx++)
    {
        QTableWidgetItem* idItem = m_ui->nodesNavigationTable->item(rowIdx, enIdColumn);
        if (idItem->text().toInt() == nodeID)
        {
            QTableWidgetItem* titleItem = m_ui->nodesNavigationTable->item(rowIdx, enTitleColumn);
            titleItem->setText(nodeNaviInfo.title);

            const QColor backgroundColor = getColorForNodeRow(nodeNaviInfo.nodeItemPtr->getNodeInfo().isValid());
            idItem->setBackgroundColor(backgroundColor);
            titleItem->setBackgroundColor(backgroundColor);
            return;
        }
    }
}

void StoryNavigationWidget::appendNodeRow(int nodeId, const QString& nodeTitle, bool nodeIsValid)
{
    QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(nodeId));
    idItem->setFont(QFont("Arial", 14, QFont::Bold, QFont::StyleItalic));
    idItem->setTextAlignment(Qt::AlignCenter);
    idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);

    QTableWidgetItem* titleItem = new QTableWidgetItem(nodeTitle);
    titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
    titleItem->setFont(QFont("Arial", 10)); // TODO подобрать font

    const QColor backgroundColor = getColorForNodeRow(nodeIsValid);
    idItem->setBackgroundColor(backgroundColor);
    titleItem->setBackgroundColor(backgroundColor);

    const int lastRowIndex = m_ui->nodesNavigationTable->rowCount();
    m_ui->nodesNavigationTable->setRowCount(lastRowIndex + 1);
    m_ui->nodesNavigationTable->setItem(lastRowIndex, enIdColumn, idItem);
    m_ui->nodesNavigationTable->setItem(lastRowIndex, enTitleColumn, titleItem);
}

void StoryNavigationWidget::refillNodeTable()
{
    clearNodeTable();
    for(NodeNavigationInfoMap::const_iterator it = m_nodeNaviInfoMap.cbegin(); it != m_nodeNaviInfoMap.cend(); ++it)
    {
        NodeNavigationInfo nodeNaviInfo = *it;
        const StoryNode& nodeInfo = nodeNaviInfo.nodeItemPtr->getNodeInfo();
        appendNodeRow(it.key(), nodeInfo.getTitle(), nodeInfo.isValid());
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
    return isValid ? QColor("#69BF83") : QColor("#B53F3F");
}

void StoryNavigationWidget::slotSearchTextChanged(const QString& searchText)
{
    bool isInteger = false;
    const int searchInt = searchText.toInt(&isInteger);
}
