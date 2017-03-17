#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <Qpen>
#include <limits>

#include "StoryScene.hpp"
#include "Items/StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"

//===================================== public ==========================================

StoryScene::StoryScene(QObject* parent) :
    QGraphicsScene(parent)
{
    /* Это конечно не круто, но ничего лучше не придумал
     * Эта точка выступает какбе якорем для правильного
     * увеличения размера сцены относительно начала координал при
     * добавлении новых элементов (в частности первого)
     */
    addLine(0, 0, 1, 1, QPen(Qt::white, 0));
    for (int i = 0; i < LIMIT_ID; i++)
    {
        m_setLimitID.insert(i);
    }
}

StoryScene::~StoryScene()
{

}

int StoryScene::nodeCount() const
{
    return getStoryNodeList().size();
}

QList<StoryNodeItem*> StoryScene::getStoryNodeList() const
{
    QList<StoryNodeItem*> storyNodes;
    foreach(auto& item, items())
    {
        StoryNodeItem* storyItem = qgraphicsitem_cast<StoryNodeItem*>(item);
        if (storyItem)
            storyNodes << storyItem;
    }
    return storyNodes;
}

//=======================================================================================

//===================================== public slots ====================================

void StoryScene::clearScene()
{
    clear();
}

//=======================================================================================

//===================================== protected =======================================

void StoryScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    if (event->mimeData()->hasFormat(StoryCommon::NODE_MIME_TYPE))
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
    if (!data->hasFormat(StoryCommon::NODE_MIME_TYPE))
        return;

    QByteArray encodedData = data->data(StoryCommon::NODE_MIME_TYPE);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QIcon icon;
    QString nodeType;
    while (!stream.atEnd())
    {
        stream >> nodeType >> icon;
    }
    QPointF p = event->scenePos();
    p.setX(p.x() - StoryNodeItem::DEFAULT_NODE_SIZE.width()/2);
    p.setY(p.y() - StoryNodeItem::DEFAULT_NODE_SIZE.height()/2);
    addStoryNode(nodeType, icon, p);
}

//=======================================================================================

//===================================== private =========================================

void StoryScene::addStoryNode(const QString& nodeType, const QIcon& icon, const QPointF& pos)
{
    StoryNodeItem* node = new StoryNodeItem(getFreeID(), nodeType);
    node->setFlag(QGraphicsItem::ItemIsMovable, true);
    node->setIcon(icon);
    node->setPos(pos);
    node->setZValue(NODE_Z_DEPTH);
    if (getStoryNodeList().isEmpty())
        node->getNodeInfo().setEntryPointFlag(true);

    // для отладки
    node->getNodeInfo().setTitle("Петуханские бои.");
    node->getNodeInfo().setText("Собрались раз семеро швабов вместе, один из них был господин \n"
                  "Шульц, другой Яккли, третий Марли, четвертый Йергли, пятый Михаль,\n"
                  " шестой Ганс, а седьмой был Вейтли; и все семеро порешили весь свет \n"
                  "обойти, поискать приключений и великие подвиги совершить. А чтоб идти \n"
                  "вооруженными и без опаски, они посчитали, что надежней всего будет заказать \n"
                  "себе одно копье на всех, да зато подлинней и покрепче. Взяли они все семеро\n"
                  " это копье, и выступил впереди самый храбрый и отважный — был то, конечно, \n"
                  "господин Шульц, а следом за ним шли один за другим все остальные, а последним был Вейтли.\n");
    //============

    addItem(node);
}

int StoryScene::getFreeID() const
{
    IDSet currIDSet;
    foreach (auto& node, getStoryNodeList())
    {
        currIDSet.insert(node->getNodeInfo().getNodeID());
    }
    IDSet resultSet = m_setLimitID - currIDSet;
    if (!resultSet.isEmpty())
        return *resultSet.begin();
    else
    {
        //TODO Выводить что-нить в лог ошибок
        return -1;
    }
}

//=======================================================================================
