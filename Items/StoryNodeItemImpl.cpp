#include <QPainter>
#include "StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"

//================================== public =============================================

StoryNodeItem::StoryNodeItem(int nodeID, const QString &typeNode, QGraphicsItem* parent) :
    QGraphicsObject(parent)
    , m_boundingRect(QRectF(0, 0, StoryGUI::NODE_ITEM_SIZE.width(), StoryGUI::NODE_ITEM_SIZE.height()))
    , m_nodeInfo(nodeID, typeNode)
{

}

StoryNodeItem::~StoryNodeItem()
{

}

void StoryNodeItem::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

StoryNode& StoryNodeItem::getNodeInfo()
{
    return m_nodeInfo;
}

StoryNode StoryNodeItem::getNodeInfo() const
{
    return m_nodeInfo;
}

//=======================================================================================

//===================================== private =========================================

QRectF StoryNodeItem::boundingRect() const
{
    return m_boundingRect;
}

void StoryNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // тут будет немного магии чисел, но уж ничего не поделать

    const qreal w = m_boundingRect.width();
    const qreal h = m_boundingRect.height();

    painter->setPen(isHeadNode() ? StoryGUI::HEAD_NODE_PEN : StoryGUI::NODE_PEN);
    painter->drawRect(QRectF(5, 5, w - 10, h - 10));

    // рисуем title и Node ID
    const QString srcTitle = m_nodeInfo.getTitle();
    // TODO сделать нормальное расположение текста на iteme
    const QString formatTitle = (srcTitle.length() > 17) ? srcTitle.left(15) + "..." : srcTitle;
    painter->drawText(QPointF(20, 20), tr("Title: ") + formatTitle);
    painter->drawText(QPointF(20, h - 15), tr("Node ID: ") + QString::number(m_nodeInfo.getId()));

    const qreal pixW = w/3;
    const qreal pixH = h/3;
    painter->drawPixmap(QPointF(w/2 - pixW/2, h/2 - pixH/4), m_icon.pixmap(pixW, pixH));
}

bool StoryNodeItem::isHeadNode() const
{
    return m_nodeInfo.getId() == StoryCommon::HEAD_NODE_ID;
}

//=======================================================================================
