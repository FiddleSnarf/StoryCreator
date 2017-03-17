#include <QPainter>
#include "StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"

//================================== public =============================================

StoryNodeItem::StoryNodeItem(QGraphicsItem* parent) :
    QGraphicsObject(parent)
    , m_boundingRect(QRectF(0, 0, 150, 150))
    , m_pen(QPen(QColor("#009966"), 2))
    , m_isEntryPoint(false)
    , m_nodeID(-1)
{

}

StoryNodeItem::~StoryNodeItem()
{

}

void StoryNodeItem::setID(const int& id)
{
    m_nodeID = id;
}

void StoryNodeItem::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

void StoryNodeItem::setType(const QString& type)
{
    m_typeNode = type;
}

void StoryNodeItem::setTitle(const QString& title)
{
    m_title = title;
}

void StoryNodeItem::setText(const QString& text)
{
    m_text = text;
}

void StoryNodeItem::setEntryPointFlag(bool state)
{
    m_isEntryPoint = state;
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

    painter->setPen(m_pen);
    painter->drawRect(QRectF(5, 5, w - 10, h - 10));

    // рисуем title и Node ID
    const QString title = (m_title.length() > 15) ? m_title.left(12) + "..." : m_title;
    painter->drawText(QPointF(20, 20), tr("Title: ") + title);
    painter->drawText(QPointF(20, h - 20), tr("Node ID: ") + QString::number(m_nodeID));

    const qreal pixW = w/3;
    const qreal pixH = h/3;
    painter->drawPixmap(QPointF(w/2 - pixW/2, h/2 - pixH/4), m_icon.pixmap(pixW, pixH));
}

//=======================================================================================
