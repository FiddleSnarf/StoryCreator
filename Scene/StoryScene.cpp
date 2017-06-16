#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

#include "StoryScene.hpp"

//===================================== public ==========================================

StoryScene::StoryScene(QObject* parent) :
    QGraphicsScene(parent)
{

}

StoryScene::~StoryScene()
{

}

int StoryScene::nodeCount() const
{
    return getStoryNodeList().size();
}

StoryNodeItemList StoryScene::getStoryNodeList() const
{
    StoryNodeItemList storyNodes;
    foreach(auto& item, items())
    {
        StoryNodeItem* storyItem = qgraphicsitem_cast<StoryNodeItemPtr>(item);
        if (storyItem)
            storyNodes << storyItem;
    }
    return storyNodes;
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

void StoryScene::setTypesNodeCollector(const StoryTypesNodeCollector& collector)
{
    m_storyTypesNodeCollector = collector;
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

//=======================================================================================

//===================================== public slots ====================================

void StoryScene::slotClearScene()
{
    //clear();
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
        if (selectedNodeItem && selectedNodeItem->isSelected())
        {
            QGraphicsScene::mousePressEvent(event);
            return;
        }

        // Снимаем выделение со всех выделенных нодов
        QList<QGraphicsItem*> selectedItemsList = selectedItems();
        for(QList<QGraphicsItem*>::iterator it = selectedItemsList.begin(); it != selectedItemsList.end(); ++it)
        {
            StoryNodeItem* nodeItem = dynamic_cast<StoryNodeItem*>(*it);
            if (nodeItem)
                nodeItem->setNodeSelection(false);
        }

        // Если нажали на нод, то выделяем его
        if (selectedNodeItem)
            selectedNodeItem->setNodeSelection(true);
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

    addItem(node);
    m_idSet.insert(newId);
    return true;
}

bool StoryScene::addStoryNode(const StoryNode& nodeInfo, const QPointF& pos)
{
    StoryNodeItem* node = new StoryNodeItem(nodeInfo);
    node->setFlag(QGraphicsItem::ItemIsMovable, true);
    node->setIcon(m_storyTypesNodeCollector.getNodeTypeInfo(nodeInfo.getType()).iconType);
    node->setPos(pos);
    node->setZValue(NODE_Z_DEPTH);

    addItem(node);
    m_idSet.insert(node->getNodeInfo().getId());
    return true;
}

//=======================================================================================
