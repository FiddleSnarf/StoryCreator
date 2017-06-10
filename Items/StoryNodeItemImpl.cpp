#include <QPainter>
#include "StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"

//================================== public =============================================

const QSize StoryNodeItem::DEFAULT_NODE_SIZE(150, 150);

StoryNodeItem::StoryNodeItem(int nodeID, const QString &typeNode, QGraphicsItem* parent) :
    QGraphicsObject(parent)
    , m_boundingRect(QRectF(0, 0, DEFAULT_NODE_SIZE.width(), DEFAULT_NODE_SIZE.height()))
    , m_pen(QPen(QColor("#009966"), 2))
    , m_penForHead(QPen(QColor("#7308d1"), 3))
    //, m_nodeInfo(nodeID, typeNode)
{

}

StoryNodeItem::~StoryNodeItem()
{

}

void StoryNodeItem::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

/*StoryCommon::NodeInfo& StoryNodeItem::getNodeInfo()
{
    return m_nodeInfo;
}

StoryCommon::NodeInfo StoryNodeItem::getNodeInfo() const
{
    return m_nodeInfo;
}*/

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

    /*const qreal w = m_boundingRect.width();
    const qreal h = m_boundingRect.height();

    painter->setPen(m_nodeInfo.getEntryPointFlag() ? m_penForHead : m_pen);
    painter->drawRect(QRectF(5, 5, w - 10, h - 10));

    // рисуем title и Node ID
    const QString srcTitle = m_nodeInfo.getTitle();
    // TODO сделать нормальное расположение текста на iteme
    const QString formatTitle = (srcTitle.length() > 17) ? srcTitle.left(15) + "..." : srcTitle;
    painter->drawText(QPointF(20, 20), tr("Title: ") + formatTitle);
    painter->drawText(QPointF(20, h - 15), tr("Node ID: ") + QString::number(m_nodeInfo.getNodeID()));

    const qreal pixW = w/3;
    const qreal pixH = h/3;
    painter->drawPixmap(QPointF(w/2 - pixW/2, h/2 - pixH/4), m_icon.pixmap(pixW, pixH));*/
}

//=======================================================================================
