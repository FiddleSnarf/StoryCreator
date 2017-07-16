#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "StoryNodeItemImpl.hpp"
#include "Common/StoryCommon.hpp"
#include "Scene/StoryScene.hpp"

//================================== public =============================================

StoryNodeItem::StoryNodeItem(const StoryNode& nodeInfo, QGraphicsItem* parent) :
    QGraphicsObject(parent),
    m_boundingRect(StoryGUI::DEFAULT_NODE_RECT),
    m_isSelected(false)
{
    const int id = nodeInfo.getId();
    if (id < 0)
    {
        StoryScene* myScene = dynamic_cast<StoryScene*>(this->scene());
        if (myScene)
        {
            m_nodeInfo = StoryNode(myScene->getFreeID(), nodeInfo.getType());
            return;
        }
    }
    m_nodeInfo = nodeInfo;
    initialization();
}

StoryNodeItem::StoryNodeItem(int nodeID, const QString &typeNode, QGraphicsItem* parent) :
    QGraphicsObject(parent),
    m_boundingRect(StoryGUI::DEFAULT_NODE_RECT),
    m_nodeInfo(nodeID, typeNode),
    m_isSelected(false)
{
    initialization();
}

StoryNodeItem::~StoryNodeItem()
{

}

void StoryNodeItem::setDefaultPen()
{
    m_borderPen = isHeadNode() ? StoryGUI::HEAD_NODE_PEN : StoryGUI::NODE_PEN;
}

void StoryNodeItem::setDefaultBrush()
{
    m_brush = isHeadNode() ? StoryGUI::HEAD_NODE_BRUSH : StoryGUI::NODE_BRUSH;
}

void StoryNodeItem::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

const StoryNode& StoryNodeItem::getNodeInfo() const
{
    return m_nodeInfo;
}

void StoryNodeItem::setNodeInfo(const StoryNode& storyNode)
{
    if (m_nodeInfo != storyNode)
    {
        m_nodeInfo = storyNode;
        emit signalDataNodeChanged(m_nodeInfo.getId());
    }
}

void StoryNodeItem::setNodeTitle(const QString& title)
{
    if (m_nodeInfo.getTitle() != title)
    {
        m_nodeInfo.setTitle(title);
        emit signalDataNodeChanged(m_nodeInfo.getId());
    }
}

void StoryNodeItem::setNodeType(const QString& type)
{
    if (m_nodeInfo.getType() != type)
    {
        m_nodeInfo.setType(type);
        emit signalDataNodeChanged(m_nodeInfo.getId());
    }
}

void StoryNodeItem::setNodeText(const QString& text)
{
    if (m_nodeInfo.getText() != text)
    {
        m_nodeInfo.setText(text);
        emit signalDataNodeChanged(m_nodeInfo.getId());
    }
}

void StoryNodeItem::setNodeActionList(const NodeActionList& actionList)
{
    if (m_nodeInfo.getNodeActionList() != actionList)
    {
        m_nodeInfo.setNodeActionList(actionList);
        emit signalDataNodeChanged(m_nodeInfo.getId());
    }
}

bool StoryNodeItem::isHeadNode() const
{
    return m_nodeInfo.getId() == StoryCommon::HEAD_NODE_ID;
}

void StoryNodeItem::setNodeSelection(bool state)
{
    if (state)
    {
        m_borderPen.setWidth(SELECTED_NODE_FRAME_WIDTH);
        m_selectedTimeline.start();
    }
    else
    {
        m_selectedTimeline.stop();
        setDefaultPen();
    }
    m_isSelected = state;
}

bool StoryNodeItem::isNodeSelected() const
{
    return m_isSelected;
}

//=======================================================================================
//===================================== private =========================================

void StoryNodeItem::initialization()
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setDefaultPen();
    setDefaultBrush();

    initSelectedPulse();

    connect(this, &QGraphicsObject::xChanged, this, &StoryNodeItem::slotGeometryNodeChanged);
    connect(this, &QGraphicsObject::yChanged, this, &StoryNodeItem::slotGeometryNodeChanged);
}

void StoryNodeItem::slotGeometryNodeChanged()
{
    emit signalGeometryNodeChanged(m_nodeInfo.getId());
}

void StoryNodeItem::initSelectedPulse()
{
    m_selectedTimeline.setDuration(TIME_LINE);
    m_selectedTimeline.setEasingCurve(QEasingCurve::Linear);
    m_selectedTimeline.setFrameRange(0, FRAMES);

    connect(&m_selectedTimeline, &QTimeLine::valueChanged, [=](double value)
    {
        QColor color;
        if (isHeadNode())
        {
            const int green = ((GREEN_MAX_HEAD - GREEN_MIN_HEAD) * value) + GREEN_MIN_HEAD;
            color = QColor(160, green, 245);
        }
        else
        {
            const int green = ((GREEN_MAX - GREEN_MIN) * value) + GREEN_MIN;
            color = QColor(70, green, 70);
        }
        m_borderPen.setColor(color);
        update();
    });

    connect(&m_selectedTimeline, &QTimeLine::finished, [=]()
    {
        if (m_selectedTimeline.direction() == QTimeLine::Forward)
            m_selectedTimeline.setDirection(QTimeLine::Backward);
        else if (m_selectedTimeline.direction() == QTimeLine::Backward)
            m_selectedTimeline.setDirection(QTimeLine::Forward);

        m_selectedTimeline.start();
    });
}

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

    painter->setBrush(m_brush);
    painter->setPen(m_borderPen);
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
//=======================================================================================
