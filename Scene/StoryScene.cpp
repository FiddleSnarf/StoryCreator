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

void StoryScene::initStoryInfo(const StoryCommon::StoryInfo& storyInfo)
{
    // TODO теперь надо добавить все ноды на сцену
}

//=======================================================================================

//===================================== public slots ====================================

void StoryScene::clearScene()
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
    pos.setX(pos.x() - StoryGUI::NODE_ITEM_SIZE.width() / 2);
    pos.setY(pos.y() - StoryGUI::NODE_ITEM_SIZE.height() / 2);
    addEmptyStoryNode(nodeType, icon, pos);
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

bool StoryScene::addStoryNode(const StoryNode& node)
{
    // TODO
    return true;
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

//=======================================================================================
