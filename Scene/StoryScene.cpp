#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QGraphicsView>

#include "StoryScene.hpp"

//===================================== public ==========================================

StoryScene::StoryScene(StoryTypesNodeCollector& collector, QObject* parent) :
    QGraphicsScene(parent),
    m_storyTypesNodeCollector(collector)
{

}

StoryScene::~StoryScene()
{

}

int StoryScene::nodeCount() const
{
    return m_nodesList.size();
}

const StoryNodeItemList& StoryScene::getStoryNodeList() const
{
    return m_nodesList;
}

StoryNodeItemPtr StoryScene::getNodeItemForID(int nodeId) const
{
    foreach(const StoryNodeItemPtr& nodeItem,m_nodesList)
    {
        if (nodeItem->getNodeInfo().getId() == nodeId)
            return nodeItem;
    }
    return StoryNodeItemPtr();
}

int StoryScene::getFreeID() const
{
    if (!m_idSet.empty())
    {
        int lastId = *m_idSet.crbegin();
        if (lastId < INT_MAX - 1)
            return ++lastId;
        else
            return StoryCommon::ERROR_NODE_ID;
    }
    return StoryCommon::HEAD_NODE_ID;
}

void StoryScene::initStoryInfo(const StoryCommon::StoryInfo& storyInfo)
{
    // Добавляем все ноды на сцену
    if (storyInfo.additionalViewParams.isValid())
    {
        foreach(const StoryNode& node, storyInfo.nodeList)
        {
            addStoryNode(node, storyInfo.additionalViewParams.nodesPosMap[node.getId()]);
        }
    }
    else
    {
        // TODO А вот тут должно быть умное авто-размещение нодов по сцене (пока что временная шляпа)
        const int SHIFT = 5;
        int counter = 0;
        QPointF pos;
        foreach(const StoryNode& node, storyInfo.nodeList)
        {
            if (counter == 30)
            {
                pos.setY(pos.y() + StoryGUI::DEFAULT_NODE_HEIGHT + SHIFT);
                pos.setX(0.);
                counter = 0;
            }
            addStoryNode(node, pos);
            pos.setX(pos.x() + StoryGUI::DEFAULT_NODE_WIDTH + SHIFT);
            counter++;
        }
    }
}

void StoryScene::selectNodeForID(int nodeId, bool centerOn)
{
    StoryNodeItemPtr storyNode = getNodeItemForID(nodeId);
    selectNode(storyNode, centerOn);
}

StoryNodeItemList StoryScene::getSelectedNodeItems() const
{
    StoryNodeItemList selectedStoryNodeItems;
    QList<QGraphicsItem*> selectedNodeItems = items();
    foreach(QGraphicsItem* item, selectedNodeItems)
    {
        StoryNodeItemPtr storyItem = dynamic_cast<StoryNodeItemPtr>(item);
        if (storyItem && storyItem->isNodeSelected())
            selectedStoryNodeItems << storyItem;
    }

    /*if (selectedStoryNodeItems.size() > 1)
        qDebug() << tr("..."); // TODO Если выделенных нодов > 1 то это не норма*/

    return selectedStoryNodeItems;
}

//=======================================================================================

//===================================== public slots ====================================

void StoryScene::slotClearScene()
{
    clear();
    m_idSet.clear();
}

//=======================================================================================

//===================================== protected =======================================

void StoryScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    if (event->mimeData()->hasFormat(StoryGUI::NODE_MIME_TYPE))
        event->acceptProposedAction();
}

void StoryScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    event->acceptProposedAction();
}

void StoryScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);
}

void StoryScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* data = event->mimeData();
    if (!data->hasFormat(StoryGUI::NODE_MIME_TYPE))
        return;

    QByteArray encodedData = data->data(StoryGUI::NODE_MIME_TYPE);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QIcon icon;
    QString nodeType;
    while (!stream.atEnd())
    {
        stream >> nodeType >> icon;
    }
    QPointF pos = event->scenePos(); // TODO Поправить рассчет координат для сцены фиксированного размера, и вообще при дропе сделать рамку показывающую куда ляжет нод
    pos.setX(pos.x() - StoryGUI::DEFAULT_NODE_WIDTH / 2);
    pos.setY(pos.y() - StoryGUI::DEFAULT_NODE_HEIGHT / 2);
    addEmptyStoryNode(nodeType, icon, pos);
}

void StoryScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        // Если нажали на уже выделенный нод, то ничего не делаем
        StoryNodeItem* selectedNodeItem = dynamic_cast<StoryNodeItem*>(itemAt(event->scenePos(), QTransform()));
        if (selectedNodeItem && selectedNodeItem->isNodeSelected())
        {
            QGraphicsScene::mousePressEvent(event);
            return;
        }
        selectNode(selectedNodeItem);
    }
    QGraphicsScene::mousePressEvent(event);
}

//=======================================================================================

//===================================== private =========================================

bool StoryScene::addEmptyStoryNode(const QString& nodeType, const QIcon& icon, const QPointF& pos)
{
    const int newId = getFreeID();
    if (newId == StoryCommon::ERROR_NODE_ID)
        return false;

    StoryNodeItem* node = new StoryNodeItem(newId, nodeType);
    node->setFlag(QGraphicsItem::ItemIsMovable, true);
    node->setIcon(icon);
    node->setPos(pos);
    node->setZValue(NODE_Z_DEPTH);

    m_nodesList << node;
    addItem(node);
    m_idSet.insert(newId);
    emit signalStoryNodeAdded(node);
    return true;
}

bool StoryScene::addStoryNode(const StoryNode& nodeInfo, const QPointF& pos)
{
    StoryNodeItem* node = new StoryNodeItem(nodeInfo);
    node->setFlag(QGraphicsItem::ItemIsMovable, true);
    node->setIcon(m_storyTypesNodeCollector.getNodeTypeInfo(nodeInfo.getType()).iconType);
    node->setPos(pos);
    node->setZValue(NODE_Z_DEPTH);

    m_nodesList << node;
    addItem(node);
    m_idSet.insert(node->getNodeInfo().getId());
    emit signalStoryNodeAdded(node);
    return true;
}

void StoryScene::selectNode(StoryNodeItem* node, bool centerOn)
{
    // Снимаем выделение со всех выделенных нодов
    foreach(StoryNodeItemPtr storyItem, getSelectedNodeItems())
    {
        storyItem->setNodeSelection(false);
    }

    // Если нажали на нод, то выделяем его
    if (node)
    {
        node->setNodeSelection(true);
        if (centerOn)
        {
            QList<QGraphicsView*> listViews = views();
            for (QList<QGraphicsView*>::iterator viewIt = listViews.begin(); viewIt != listViews.end(); ++viewIt)
            {
                (*viewIt)->centerOn(node);
            }
        }
        emit signalItemSelectedChanged(true, node);
    }
    else
        emit signalItemSelectedChanged(false, nullptr);
}

//=======================================================================================
